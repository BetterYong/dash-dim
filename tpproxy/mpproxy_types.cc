#include <unistd.h>
#include <memory.h>
#include <algorithm>
#include <limits>
#include "sequencer/quic_types.h"
#include "base/epoll_api.h"
#include "tpproxy/mpproxy_types.h"
#include "tpproxy/windowed_filter.h"
#include "tcp/tcp_info.h"
#include "base/byte_codec.h"
#include  "logging/logging.h"
namespace basic{
const QuicTime::Delta kGoodputInterval=QuicTime::Delta::FromMilliseconds(100);
const QuicTime::Delta kBudgetInterval=QuicTime::Delta::FromMilliseconds(10);
const QuicTime::Delta kScheduleInterval=QuicTime::Delta::FromMilliseconds(10);
const QuicBandwidth kMinGoodput=QuicBandwidth::FromKBitsPerSecond(500);
const QuicTime::Delta kHealthInterval=QuicTime::Delta::FromMilliseconds(2000);
const int kBandwithWindowSize=10;
const int kWriteBufferThreshold=1500*10;
const int kMinPayloadSize=1500;
const double kBandwidthGain=1.25;
bool IsDeliverable(const char *data,size_t size,uint64_t *offset,uint16_t *offset_bytes,uint64_t *len,uint16_t *len_bytes){
    bool ret=false;
    if(size>0){
    DataReader reader(data,size);
    bool success=reader.ReadVarInt62(offset)&&reader.ReadVarInt62(len);
    if(success){
        *offset_bytes=DataWriter::GetVarInt62Len(*offset);
        *len_bytes=DataWriter::GetVarInt62Len(*len);
        size_t frame_size=(*offset_bytes)+(*len_bytes)+(*len);
        if(size>=frame_size){
            ret=true;
        }
    }
    }
    if(!ret){
        *offset=0;
        *offset_bytes=0;
        *len=0;
        *len_bytes=0;
    }
    return ret;
}
SocketBase::SocketBase(basic::BaseContext *context,int fd):context_(context),fd_(fd){}
void SocketBase::FlushBuffer(){
    if(fd_<0){
        return ;
    }
    size_t remain=fd_write_buffer_.size();
    const char *data=fd_write_buffer_.data();
    bool flushed=false;
    while(remain>0){
        size_t intend=std::min(kBufferSize,remain);
        if(CanSend(intend)){
            int sent=send(fd_,data,intend,0);
            if(sent<=0){
                break;
            }
            send_bytes_+=sent;
            flushed=true;
            data+=sent;
            remain-=sent;
            OnPacketSent(sent);
        }else{
            break;
        }

    }
    if(flushed){
        if(remain>0){
            std::string copy(data,remain);
            copy.swap(fd_write_buffer_);
        }else{
            std::string null_str;
            null_str.swap(fd_write_buffer_);
        }        
    }
}
void SocketBase::OnWriteEvent(int fd){
    FlushBuffer();
    if(fd_write_buffer_.size()>0){
       context_->epoll_server()->ModifyCallback(fd_,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR); 
    }else{
        context_->epoll_server()->ModifyCallback(fd_,EPOLLET|EPOLLIN|EPOLLRDHUP|EPOLLERR);
        if((!(io_status_&FD_WAIT_CLOSE))&&CanSend(0)){
            OnCanSend();
        }
    }
    CheckCloseFd();
}
void SocketBase::CheckCloseFd(){
    if((io_status_&FD_WAIT_CLOSE)&&(fd_write_buffer_.size()==0)){
        CloseFd();
    }
}
void SocketBase::CheckToDeleteSelf(){
    if((io_status_&FD_CLOSED)&&(io_status_&ENTITY_CLOSED)){
        DeleteSelf();
    }
}
void SocketBase::CloseFd(){
    if(fd_>0){
        io_status_&=(~ENTITY_WAIT_CLOSE_IF_FD_CLOSED);
        io_status_|=ENTITY_WAIT_CLOSE_IF_FD_CLOSED;
        context_->epoll_server()->UnregisterFD(fd_);        
        close(fd_);
        fd_=-1;
    }
    CheckToDeleteSelf(); 
}
void SocketBase::DeleteSelf(){
    if(destroyed_){
        return;
    }
    destroyed_=true;
    context_->PostTask([this]{
        delete this;
    });
}

class GoogputAlarmDelegate:public BaseAlarm::Delegate{
public:
    GoogputAlarmDelegate(TcpConnectionBase *entity):entity_(entity){}
    void OnAlarm() override{
        entity_->OnGoodputAlarm();
    }
private:
    TcpConnectionBase *entity_=nullptr;    
};
class BudgetAlarmDelegate:public BaseAlarm::Delegate{
public:
    BudgetAlarmDelegate(TcpConnectionBase *entity):entity_(entity){}
    void OnAlarm() override{
        entity_->OnBudgetAlarm();
    }
private:
    TcpConnectionBase *entity_=nullptr;    
};

TcpConnectionBase::TcpConnectionBase(BaseContext *context,int fd):SocketBase(context,fd),
max_goodput_(kBandwithWindowSize,QuicBandwidth::Zero(),0),
packet_budget_(0){
    struct tcp_info_copy info;
    memset(&info,0,sizeof(info));
    socklen_t info_size=sizeof(info);
    uint64_t bytes_acked=0;
    if(getsockopt(fd_,IPPROTO_TCP,TCP_INFO,(void*)&info,&info_size)==0){
        bytes_acked=info.tcpi_bytes_acked;
    }
    QuicTime now=context_->clock()->ApproximateNow();
    last_bytes_acked_=bytes_acked;
    last_goodput_time_=now;
    last_budget_time_=now;
    goodput_alarm_.reset(context_->alarm_factory()->CreateAlarm(new GoogputAlarmDelegate(this)));
    goodput_alarm_->Update(now+kGoodputInterval,QuicTime::Delta::Zero());
    QuicBandwidth target=kBandwidthGain*GoodputWithMinimum();
    packet_budget_.set_target_rate_kbps(target.ToKBitsPerSecond());
    budget_alarm_.reset(context_->alarm_factory()->CreateAlarm(new BudgetAlarmDelegate(this)));
    budget_alarm_->Update(now+kBudgetInterval,QuicTime::Delta::Zero());
    packet_budget_.IncreaseBudget(kBudgetInterval.ToMilliseconds());
}
TcpConnectionBase::~TcpConnectionBase(){
    if(budget_alarm_){
        budget_alarm_->Cancel();
    }
    if(goodput_alarm_){
        goodput_alarm_->Cancel();
    }
}
//offset+len+pv
void TcpConnectionBase::SendData(uint64_t offset,const char *pv,size_t size){
    CHECK(fd_>0);
    //std::cout<<"SendData "<<fd_<<" "<<size<<std::endl;
    CHECK(size<=std::numeric_limits<quic::QuicPacketLength>::max());
    size_t offset_bytes=DataWriter::GetVarInt62Len(offset);
    size_t size_bytes=DataWriter::GetVarInt62Len(size);
    size_t buffer_size=offset_bytes+size_bytes+size;
    size_t old_size=fd_write_buffer_.size();
    fd_write_buffer_.resize(old_size+buffer_size);
    DataWriter writer(&fd_write_buffer_[old_size],buffer_size);
    bool success=writer.WriteVarInt62(offset)&&writer.WriteVarInt62(size)&&
                writer.WriteBytes((const void*)pv,size);
    CHECK(success);
    if(IsConnected()){
        OnWriteEvent(fd_);
    }
}
void TcpConnectionBase::OnGoodputAlarm(){
    struct tcp_info_copy info;
    memset(&info,0,sizeof(info));
    socklen_t info_size=sizeof(info);
    uint64_t bytes_acked=0;
    if(getsockopt(fd_,IPPROTO_TCP,TCP_INFO,(void*)&info,&info_size)==0){
        bytes_acked=info.tcpi_bytes_acked;
    }
    QuicTime now=context_->clock()->ApproximateNow();
    goodput_alarm_->Update(now+kGoodputInterval,QuicTime::Delta::Zero());
    QuicBandwidth b=QuicBandwidth::Zero();
    if((bytes_acked>last_bytes_acked_)&&(now>last_goodput_time_)){
        b=QuicBandwidth::FromBytesAndTimeDelta(bytes_acked-last_bytes_acked_,now-last_goodput_time_);
    }
    //std::cout<<"bw "<<round_trip_count_<<" "<<b.ToKBitsPerSecond()<<"kbps"<<std::endl;
    max_goodput_.Update(b,round_trip_count_);
    QuicBandwidth target=kBandwidthGain*GoodputWithMinimum();
    packet_budget_.set_target_rate_kbps(target.ToKBitsPerSecond());
    round_trip_count_++;
    last_bytes_acked_=bytes_acked;
    last_goodput_time_=now;
}
void TcpConnectionBase::OnBudgetAlarm(){
    QuicTime now=context_->clock()->ApproximateNow();
    if(now>last_budget_time_){
        QuicBandwidth target=kBandwidthGain*GoodputWithMinimum();
        packet_budget_.set_target_rate_kbps(target.ToKBitsPerSecond());
        int64_t delta_time_ms=(now-last_budget_time_).ToMilliseconds();
        packet_budget_.IncreaseBudget(delta_time_ms);
    }
    if((packet_budget_.bytes_remaining()>0)&&(fd_write_buffer_.size()==0)){
        OnCanSend();
    }
    last_budget_time_=now;
    budget_alarm_->Update(now+kBudgetInterval,QuicTime::Delta::Zero());
}
QuicBandwidth TcpConnectionBase::GoodputWithMinimum() const{
    QuicBandwidth b=max_goodput_.GetBest();
    if(b<kMinGoodput){
        b=kMinGoodput;
    }
    return b;
}
bool TcpConnectionBase::IsBufferAboveThreshold() const{
    bool ret=false;
    if(fd_write_buffer_.size()>kWriteBufferThreshold){
        ret=true;
    }
    return ret;
}
void TcpConnectionBase::WriteMessage(uint8_t type,const char *data,size_t size){
    char buffer[kBufferSize];
    uint64_t offset=0;
    size_t len=sizeof(type)+size;
    size_t offset_bytes=DataWriter::GetVarInt62Len(offset);
    size_t len_bytes=DataWriter::GetVarInt62Len(len);
    DataWriter writer(buffer,kBufferSize);
    bool success=writer.WriteVarInt62(offset)&&writer.WriteVarInt62((uint64_t)len)&&
                writer.WriteUInt8(type);
    CHECK(success);
    if(size>0&&data){
        success=writer.WriteBytes((const void*)data,size);
        CHECK(success);
    }
    size_t old_size=fd_write_buffer_.size();
    fd_write_buffer_.resize(old_size+writer.length());
    memcpy(&fd_write_buffer_[old_size],writer.data(),writer.length());
    if(IsConnected()){
        OnWriteEvent(fd_);
    }
}
bool TcpConnectionBase::CanSend(int64_t bytes) const{
    bool ret=false;
    if(packet_budget_.IsStarted()){
        if(packet_budget_.bytes_remaining()>0){
            ret=true;
        }
    }else{
        ret=true;
    }
    return ret;
}
void TcpConnectionBase::OnCanSend(){
//TODO
//may send message to server
}
void TcpConnectionBase::OnPacketSent(int64_t bytes){
    if(packet_budget_.IsStarted()){
        packet_budget_.UseBudget(bytes);
    }   
}
void TcpConnectionBase::OnReadEvent(int fd){
    char buffer[kBufferSize+500];
    bool update=false;
    while(true){
        int nbytes=read(fd,buffer,kBufferSize+500);  
        if (nbytes == -1) {
            //if(errno == EWOULDBLOCK|| errno == EAGAIN){}
            break;            
        }else if(nbytes==0){
            CloseFd();
            CHECK(fd_write_buffer_.size()==0);
        }else{
            recv_bytes_+=nbytes;
            size_t old=fd_read_buffer_.size();
            fd_read_buffer_.resize(old+nbytes);
            memcpy(&fd_read_buffer_[old],buffer,nbytes);
            update=true;
        }       
    }
    if(update){
        Deliver();
    }
    if(io_status_&FD_CLOSED){
        std::cout<<fd<<" "<<fd_read_buffer_.size()<<std::endl;
        CHECK(fd_read_buffer_.size()==0);
    }
}
void TcpConnectionBase::Deliver(){
    const char *data=fd_read_buffer_.data();
    size_t remain=fd_read_buffer_.size();
    uint64_t offset=0;
    uint16_t offset_bytes=0;
    uint64_t len=0;
    uint16_t len_bytes=0;
    bool update=false;
    while((remain>0)&&IsDeliverable(data,remain,&offset,&offset_bytes,&len,&len_bytes)){
        size_t header_size=offset_bytes+len_bytes;
        size_t frame_size=header_size+(size_t)len;
        const char*pv=data+header_size;
        if(offset>=kReserveOffset){
            CHECK(server_);
            server_->RecvData(offset,pv,(size_t)len);
        }else{
            if(len>0){
                ParseMessage(offset,pv,(size_t)len);
            }
        }
        data+=frame_size;
        remain-=frame_size;
        update=true;
    }
    if(update){
        if(remain>0){
            std::string copy(data,remain);
            copy.swap(fd_read_buffer_);
        }else{
            std::string null_str;
            null_str.swap(fd_read_buffer_);
            CheckCloseEntity();
        }        
    }
}
void TcpConnectionBase::CheckCloseEntity(){
    if((io_status_&ENTITY_WAIT_CLOSE)&&(fd_read_buffer_.size()==0)){
        CloseEntity();
    }
}
void TcpConnectionBase::CloseEntity(){
    io_status_&=(~ENTITY_CLOSED);
    io_status_|=ENTITY_CLOSED;
    if(server_){
        server_->Notify(this,PROXY_CLOSE);
        server_=nullptr;
    }
    CheckToDeleteSelf();
}

class ServerBudgetAlarmDelegate:public BaseAlarm::Delegate{
public:
    ServerBudgetAlarmDelegate(TpServerBase *entity):entity_(entity){}
    void OnAlarm() override{
        entity_->OnBudgetAlarm();
    }
private:
    TpServerBase *entity_=nullptr;    
};
class HealthAlarmDelegate:public BaseAlarm::Delegate{
public:
    HealthAlarmDelegate(TpServerBase *entity):entity_(entity){}
    void OnAlarm() override{
        entity_->OnHealthAlarm();
    }
private:
    TpServerBase *entity_=nullptr;    
};
TpServerBase::TpServerBase(BaseContext *context,int fd):SocketBase(context,fd),sequencer_(this),
packet_budget_(0){}
TpServerBase::~TpServerBase(){
    std::cout<<"~TpServerBase: "<<sequencer_bytes_<<std::endl;
    if(budget_alarm_){
        budget_alarm_->Cancel();
    }
    if(health_alarm_){
        health_alarm_->Cancel();
    }
}
void TpServerBase::RecvData(uint64_t offset,const char *pv,uint64_t size){
    if(offset>=kReserveOffset){
        offset-=kReserveOffset;
        quic::QuicStreamFrame frame(stream_id_,false,offset,pv,(uint16_t)size);
        sequencer_.OnStreamFrame(frame);
    }
}
void TpServerBase::OnDataAvailable(){
    CreateHealthAlarm();
    last_health_time_=context_->clock()->ApproximateNow();
    std::string buffer;
    sequencer_.Read(&buffer);
    sequencer_bytes_+=buffer.size();
    WriteDataToFd(buffer.data(),buffer.size());
}
void TpServerBase::OnUnrecoverableError(quic::QuicErrorCode error,const std::string& details){
    std::cout<<"sequencer error: "<<details<<std::endl;
}
void TpServerBase::OnBudgetAlarm(){
    QuicTime now=context_->clock()->ApproximateNow();
    UpdateReadBudgetRate();
    if(last_budget_time_==QuicTime::Zero()){
        packet_budget_.IncreaseBudget(kScheduleInterval.ToMilliseconds());
    }else{
        if(now>last_budget_time_){
            int64_t delta_time_ms=(now-last_budget_time_).ToMilliseconds();
            packet_budget_.IncreaseBudget(delta_time_ms);
        }else{
            packet_budget_.IncreaseBudget(kScheduleInterval.ToMilliseconds());
        }
    }
    last_budget_time_=now;
    budget_alarm_->Update(now+kScheduleInterval,QuicTime::Delta::Zero());
    int64_t target=packet_budget_.bytes_remaining();
    bool buffer_full=false;
    int buffer_full_connections=0;
    for (auto it=ready_list_.begin();it!=ready_list_.end();it++){
        TcpConnectionBase *connection=(*it);
        if(connection->IsBufferAboveThreshold()){
            buffer_full_connections++;
        }
    }
    if(buffer_full_connections==ready_list_.size()){
        buffer_full=true;
    }
    if(fd_>0&&(!buffer_full)){
        OnReadEvent(fd_);
    }
    if(fd_read_buffer_.size()>0){
        Schedule();
    }
    if(io_status_&FD_CLOSED){
        CheckCloseEntity();
    }
}
void TpServerBase::OnHealthAlarm(){
    QuicTime now=context_->clock()->ApproximateNow();
    if(last_health_time_==QuicTime::Zero()){
        last_health_time_=now;
    }
    if(now>=last_health_time_+kHealthInterval){
        health_time_out_++;
        std::cout<<"health time out "<<health_time_out_<<" "<<sequencer_.NumBytesBuffered()<<std::endl;
    }else{
        health_time_out_=0;
    }
    health_alarm_->Update(now+kHealthInterval,QuicTime::Delta::Zero());
}
void TpServerBase::CreateBudgetAlarm(){
    if(!budget_alarm_){
        QuicTime now=context_->clock()->ApproximateNow();
        budget_alarm_.reset(context_->alarm_factory()->CreateAlarm(new ServerBudgetAlarmDelegate(this)));
        UpdateReadBudgetRate();
        budget_alarm_->Update(now,QuicTime::Delta::Zero());
    }
}
void TpServerBase::CreateHealthAlarm(){
    if(!health_alarm_){
        QuicTime now=context_->clock()->ApproximateNow();
        health_alarm_.reset(context_->alarm_factory()->CreateAlarm(new HealthAlarmDelegate(this)));
        health_alarm_->Update(now,QuicTime::Delta::Zero());
    }
}
void TpServerBase::UpdateReadBudgetRate(){
    QuicBandwidth b=QuicBandwidth::Zero();
    for(auto it=ready_list_.begin();it!=ready_list_.end();it++){
        TcpConnectionBase *connection=(*it);
        if(!connection->IsBufferAboveThreshold()){
            b=b+kBandwidthGain*connection->GoodputWithMinimum();
        }
    }
    if(b<kMinGoodput){
        b=kMinGoodput;
    }
    packet_budget_.set_target_rate_kbps(b.ToKBitsPerSecond());
}
void TpServerBase::OnReadEvent(int fd){
    char buffer[kBufferSize];
    int64_t read_bytes=0;
    int64_t target=packet_budget_.bytes_remaining();
    if(target<0) {return ;}
    while(true){
        int nbytes=read(fd,buffer,kBufferSize);  
        if (nbytes == -1) {
            //if(errno == EWOULDBLOCK|| errno == EAGAIN){}
            break;            
        }else if(nbytes==0){
            CloseFd();
        }else{
            recv_bytes_+=nbytes;
            read_bytes+=nbytes;
            OnDataFromFd(buffer,nbytes);
            if(read_bytes>=target){
                break;
            }
        }
    }
    if(read_bytes>0){
        packet_budget_.UseBudget(read_bytes);
    }
    CheckCloseEntity();
}
size_t TpServerBase::WriteDataToFd(const char *pv,size_t size){
    if(fd_<0){
        return 0;
    }
    FlushBuffer();
    size_t old_size=fd_write_buffer_.size();
    if(old_size>0){
        fd_write_buffer_.resize(old_size+size);
        memcpy(&fd_write_buffer_[old_size],pv,size);
    }else if(old_size==0){
        int sent=send(fd_,pv,size,0);
        const char *data=pv;
        size_t remain=size;
        if(sent>0){
            data+=sent;
            send_bytes_+=sent;
            remain=size-sent;
        }
        CHECK(remain>=0);
        if(remain>0){
            fd_write_buffer_.resize(old_size+remain);
            memcpy(&fd_write_buffer_[old_size],data,remain);
            context_->epoll_server()->ModifyCallback(fd_,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
        }
    }
    return size;
}
void TpServerBase::OnDataFromFd(const char *pv,size_t size){
    CreateHealthAlarm();
    last_health_time_=context_->clock()->ApproximateNow();
    size_t old_size=fd_read_buffer_.size();
    fd_read_buffer_.resize(old_size+size);
    memcpy(&fd_read_buffer_[old_size],pv,size);
}
void TpServerBase::Schedule(){
    std::vector<TcpConnectionBase*> connections;
     QuicBandwidth total=QuicBandwidth::Zero();
    for(auto it=ready_list_.begin();it!=ready_list_.end();it++){
        TcpConnectionBase* c=(*it);
        if(!c->IsBufferAboveThreshold()){
            connections.push_back(c);
            total=total+c->GoodputWithMinimum();
        }
    }
    int available=connections.size();
    if(0==available){
        return;
    }
    const char *data=fd_read_buffer_.data();
    size_t remain=fd_read_buffer_.size();
    bool flushed=false;
    if(available>1){
        for(int i=0;i<available-1;i++){
            if(remain<=0){
                break;
            }
            TcpConnectionBase *connection=connections[i];
            double ratio=1.0*connection->GoodputWithMinimum().ToKBitsPerSecond()/total.ToKBitsPerSecond();
            size_t target=remain*ratio;
            target=std::max(target,(size_t)kMinPayloadSize);
            size_t sent=std::min(target,remain);
            SendDataWithLimit(connection,data,sent);
            flushed=true;
            data+=sent;
            remain-=sent;
        }        
    }
    if(remain>0){
        TcpConnectionBase* connection=connections[available-1];
        size_t sent=remain;
        SendDataWithLimit(connection,data,sent);
        flushed=true;
        data+=sent;
        remain-=sent;
    }
    if(flushed){
        if(remain>0){
            std::string copy(data,remain);
            copy.swap(fd_read_buffer_);
        }else{
            std::string null_str;
            null_str.swap(fd_read_buffer_);
        }
    }
}
void TpServerBase::SendDataWithLimit(TcpConnectionBase *connection,const char *pv,size_t size){
    size_t remain=size;
    size_t limit=std::numeric_limits<quic::QuicPacketLength>::max();
    while(remain>0){
        size_t sent=std::min(remain,limit);
        connection->SendData(write_offset_,pv,sent);
        pv+=sent;
        remain-=sent;
        write_offset_+=sent;
    }
}
bool TpServerBase::RemoveFromReadyList(TcpConnectionBase* connection){
    bool removed=false;
    if(ready_list_.size()>0){
        std::deque<TcpConnectionBase*>::iterator it=ready_list_.begin();
        while(it!=ready_list_.end()){
            if((*it)==connection){
                it =ready_list_.erase(it);
                removed=true;
                break;
            }else{
                it++;
            }
        }        
    }
    return removed;
}
void TpServerBase::CheckCloseEntity(){
    if((io_status_&ENTITY_WAIT_CLOSE)&&(fd_read_buffer_.size()==0)){
        CloseEntity();
    }
}
void TpServerBase::CloseEntity(){
    io_status_&=(~ENTITY_CLOSED);
    io_status_|=ENTITY_CLOSED;
    for(auto it=ready_list_.begin();it!=ready_list_.end();it++){
        TcpConnectionBase *connection=(*it);
        connection->Notify(PROXY_CLOSE);
    }
    ready_list_.clear();
    CheckToDeleteSelf();    
}
}
