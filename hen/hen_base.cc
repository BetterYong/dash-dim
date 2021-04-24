#include <memory.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/netfilter_ipv4.h>
#include "hen/hen_base.h"
#include "base/byte_codec.h"
#include "logging/logging.h"
namespace basic{
#define FUN_LINE __FUNCTION__<<__LINE__<<" "
namespace{
    static int kHenVerbosity=0;
    const int kBufferSize=1500;
    static bool kProxyMode=true;
    static sockaddr_storage kCHenAddr;
    static bool kEpollET=false;
}
void hen_verbose(int v){
    kHenVerbosity=v;
}
void disable_proxy_mode(sockaddr_storage addr){
    kCHenAddr=addr;
    kProxyMode=false;
}
void enable_epoll_et(){
    kEpollET=true;
}
HenBase::HenBase(BaseContext *context,int fd)
:context_(context),fd_(fd){}
void HenBase::Sink(const char*pv,int sz){
    if(fd_<0){
        return ;
    }
    //std::cout<<Name()<<" sink "<<sz<<std::endl;
    if(pv&&sz>0){
        int old=wb_.size();
        wb_.resize(old+sz);
        memcpy(&wb_[old],pv,sz);
    }
    if(own_status_==STICK_CONNECTED){
       // FlushBuffer();
    }
    
}
void HenBase::FlushBuffer(){
    if(fd_<0){
        CHECK(wb_.size()==0);
        return ;
    }
    int remain=wb_.size();
    const char *data=wb_.data();
    bool flushed=false;
    while(remain>0){
        int target=std::min(kBufferSize,remain);
        int n=send(fd_,data,target,0);
        CHECK(n<=target);
        if(n<=0){
            break;
        }
        send_bytes_+=n;
        flushed=true;
        data+=n;
        remain-=n;
    }
    if(flushed){
        if(remain>0){
            std::string copy(data,remain);
            copy.swap(wb_);
        }else{
            std::string null_str;
            null_str.swap(wb_);
        }
    }
}
void HenBase::OnReadEvent(int fd){
    char buffer[kBufferSize];
    while(true){
        int n=read(fd,buffer,kBufferSize);  
        if (n== -1) {
            //if(errno == EWOULDBLOCK|| errno == EAGAIN){}
            break;            
        }else if(n==0){
            CloseFd();
        }else{
            recv_bytes_+=n;
            if(stick_){
                stick_->Sink(buffer,n);
            }else{
                CloseFd();
                break;
            }
        }
    }
}
void HenBase::OnWriteEvent(int fd){
    std::cout<<Name()<< "on write event"<<std::endl;
    FlushBuffer();
    CheckCloseFd();
}
void HenBase::CheckCloseFd(){
    if(wb_.size()==0 &&stick_status_==STICK_DISCON){
        CloseFd();
    }
}
void HenBase::CloseFd(){
    if(fd_>0){
        context_->epoll_server()->UnregisterFD(fd_);
        close(fd_);
        fd_=-1;
    }
    if(stick_){
        stick_->Notify(STICK_DISCON);
        stick_=nullptr;
    }
    DeleteSelf();
}
void HenBase::DeleteSelf(){
    if(destroyed_){
        return;
    }
    destroyed_=true;
    context_->PostTask([this]{
        delete this;
    });
}
BHenLeftStick::BHenLeftStick(BaseContext *context,int fd,
            const sockaddr_storage &b_hen_addr,
            const sockaddr_storage &c_hen_addr):HenBase(context,fd){
    if(kEpollET){
        context_->epoll_server()->RegisterFD(fd_,this,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }else{
        context_->epoll_server()->RegisterFD(fd_,this,EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }
    sockaddr_storage origin_dst;
    socklen_t n=sizeof(origin_dst);
    own_status_=STICK_CONNECTED;
    if(kProxyMode){
        int ret =getsockopt(fd_, SOL_IP, SO_ORIGINAL_DST, &origin_dst, &n);
        if(ret!=0){
            CloseFd();
            return;
        }
    }else{
        origin_dst=kCHenAddr;
    }
    int sock=socket(AF_INET, SOCK_STREAM, 0);
    if (sock<0){
        CloseFd();
        return;
    }
    BHenRightStick *right=new BHenRightStick(context,sock,
            b_hen_addr,c_hen_addr,origin_dst);
    if(right->AsynConnect()){
        right->set_stick(this);
        stick_=right;
    }else{
        CloseFd();
        return;
    }
}
BHenLeftStick::~BHenLeftStick(){
    std::cout<<Name()<<" dtor"<<send_bytes_<<" "<<recv_bytes_<<std::endl;
}
void BHenLeftStick::Notify(StickStatus status){
    stick_status_=status;
    if(stick_status_==STICK_DISCON){
        stick_=nullptr;
    }
}
void BHenLeftStick::OnEvent(int fd, basic::EpollEvent* event){
    if(event->in_events & EPOLLIN){
        int n=0,last=0;
        if(kHenVerbosity){
            last=recv_bytes_;
        }
        OnReadEvent(fd);
        if(kHenVerbosity){
            n=recv_bytes_-last;
            std::cout<<FUN_LINE<<" read "<<n<<std::endl;
        }
    }
    if(event->in_events&EPOLLOUT){
        OnWriteEvent(fd);
    }
}
void BHenLeftStick::OnShutdown(basic::EpollServer* eps, int fd){
    CloseFd();
}
BHenRightStick::BHenRightStick(BaseContext *context,int fd,
        const sockaddr_storage &b_hen_addr,
        const sockaddr_storage &c_hen_addr,
        const sockaddr_storage &origin_dst):
                HenBase(context,fd),
                b_hen_addr_(b_hen_addr),
                c_hen_addr_(c_hen_addr){
    char meta[6]={0};
    DataWriter writer(meta,6);
    SocketAddress socket_addr(origin_dst);
    IpAddress host=socket_addr.host();
    in_addr ip4=host.GetIPv4();
    uint32_t ip_buf;
    memcpy((void*)&ip_buf,(void*)&ip4,sizeof(in_addr));
    uint16_t port=socket_addr.port();
    bool success=writer.WriteUInt32(ip_buf)&&
            writer.WriteUInt16(port);
    Sink(meta,6);
}
BHenRightStick::~BHenRightStick(){
    std::cout<<Name()<<" dtor"<<send_bytes_<<" "<<recv_bytes_<<std::endl;
}
bool BHenRightStick::AsynConnect(){
    int yes=1;
    bool success=false;
    size_t addr_size = sizeof(struct sockaddr_storage);
    if(setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))!=0){
        CloseFd();
        return success;
    }
    if(bind(fd_, (struct sockaddr *)&b_hen_addr_, addr_size)<0){
        CloseFd();
        return success;
    }
    if(kEpollET){
        context_->epoll_server()->RegisterFD(fd_, this,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }else{
        context_->epoll_server()->RegisterFD(fd_, this,EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }
    if(connect(fd_,(struct sockaddr *)&c_hen_addr_,addr_size) == -1&& errno != EINPROGRESS){
        //connect doesn't work, are we running out of available ports ? if yes, destruct the socket
        if (errno == EAGAIN){
            CloseFd();
            return success;
        }
    }
    own_status_=STICK_CONNECTING;
    return true;
}
void BHenRightStick::Notify(StickStatus status){
    stick_status_=status;
    if(stick_status_==STICK_DISCON){
        stick_=nullptr;
    }
}
void BHenRightStick::OnShutdown(basic::EpollServer* eps, int fd){
    CloseFd();
}
void BHenRightStick::OnEvent(int fd, basic::EpollEvent* event){
    if (event->in_events&(EPOLLERR|EPOLLRDHUP|EPOLLHUP)){
        if (own_status_==STICK_CONNECTING){
            std::cout<<FUN_LINE<<"connectd to c failed"<<std::endl;
        }
        CloseFd();
    }
    if(event->in_events&EPOLLIN){
        int n=0,last=0;
        if(kHenVerbosity){
            last=recv_bytes_;
        }
        OnReadEvent(fd);
        if(kHenVerbosity){
            n=recv_bytes_-last;
            std::cout<<FUN_LINE<<" read "<<n<<std::endl;
        }
    }
    if(event->in_events&EPOLLOUT){
        if(own_status_==STICK_CONNECTING){
            own_status_=STICK_CONNECTED;
            if(stick_){
                stick_->Notify(STICK_CONNECTED);
            }
            std::cout<<FUN_LINE<<"c connected"<<std::endl;
        }
        OnWriteEvent(fd);
    }
}
void BHenBackend::CreateEndpoint(BaseContext *context,int fd){
    BHenLeftStick *stick=new BHenLeftStick(context,fd,b_hen_addr_,
            c_hen_addr_);
    UNUSED(stick);
}
PhysicalSocketServer* BHenSocketFactory::CreateSocketServer(BaseContext *context){
    std::unique_ptr<BHenBackend> backend(new BHenBackend(b_hen_addr_,c_hen_addr_));
    return new PhysicalSocketServer(context,std::move(backend));
}

CHenLeftStick::CHenLeftStick(BaseContext *context,int fd):HenBase(context,fd){
    own_status_=STICK_CONNECTED;
    if(kEpollET){
        context_->epoll_server()->RegisterFD(fd_,this,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }else{
        context_->epoll_server()->RegisterFD(fd_,this,EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }
    
}
CHenLeftStick::~CHenLeftStick(){
    std::cout<<Name()<<" dtor"<<send_bytes_<<" "<<recv_bytes_<<std::endl;
}
void CHenLeftStick::Notify(StickStatus status){
    stick_status_=status;
    if(stick_status_==STICK_DISCON){
        stick_=nullptr;
    }
}
void CHenLeftStick::OnEvent(int fd, basic::EpollEvent* event){
    if(event->in_events & EPOLLIN){
        int n=0,last=0;
        if(kHenVerbosity){
            last=recv_bytes_;
        }
        OnReadEvent(fd);
        if(kHenVerbosity){
            n=recv_bytes_-last;
            std::cout<<FUN_LINE<<" read "<<n<<std::endl;
        }
    }
    if(event->in_events&EPOLLOUT){
        OnWriteEvent(fd);
    }
}
void CHenLeftStick::OnShutdown(basic::EpollServer* eps, int fd){
    CloseFd();
}
void CHenLeftStick::OnReadEvent(int fd){
    char buffer[kBufferSize];
    while(true){
        int n=read(fd,buffer,kBufferSize);
        if (n== -1) {
            //if(errno == EWOULDBLOCK|| errno == EAGAIN){}
            break;
        }else if(n==0){
            CloseFd();
        }else{
            recv_bytes_+=n;
            if(kHenVerbosity){
                std::cout<<FUN_LINE<<" read "<<n<<std::endl;
            }
            if (!meta_parsed_){
                int old=meta_buf_.size();
                meta_buf_.resize(old+n);
                memcpy(&meta_buf_[old],buffer,n);
                int bz=meta_buf_.size();
                if(bz>=6){
                    CreateRightStick();
                    CHECK(stick_);
                    if(bz>6){
                        stick_->Sink(&meta_buf_[6],bz-6);
                    }
                    meta_parsed_=true;
                }
            }else{
                if(stick_){
                    stick_->Sink(buffer,n);
                }else{
                    CloseFd();
                    break;
                }
            }

        }
    }
}
void CHenLeftStick::CreateRightStick(){
    DataReader reader(meta_buf_.data(),6);
    in_addr ip4;
    uint32_t ip_temp;
    uint16_t port=0;
    bool success=reader.ReadUInt32(&ip_temp)&&reader.ReadUInt16(&port);
    memcpy((void*)&ip4,(void*)&ip_temp,sizeof(in_addr));
    IpAddress ip_addr(ip4);
    SocketAddress socket_addr(ip_addr,port);
    sockaddr_storage origin_dst=socket_addr.generic_address();
    std::cout<<"orgin dst "<<socket_addr.ToString()<<std::endl;
    int sock=socket(AF_INET, SOCK_STREAM,0);
    if (sock<0){
        CloseFd();
        return;
    }
    CHenRightStick *right=new CHenRightStick(context_,sock,origin_dst);
    if(right->AsynConnect()){
        right->set_stick(this);
        stick_=right;
    }else{
        CloseFd();
        return;
    }
}

CHenRightStick::CHenRightStick(BaseContext *context,int fd,
            sockaddr_storage &origin_dst)
:HenBase(context,fd),origin_dst_(origin_dst){}
CHenRightStick::~CHenRightStick(){
    std::cout<<Name()<<" dtor"<<send_bytes_<<" "<<recv_bytes_<<std::endl;
}
bool CHenRightStick::AsynConnect(){
    int yes=1;
    bool success=false;
    size_t addr_size = sizeof(struct sockaddr_storage);
    if(setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))!=0){
        CloseFd();
        return success;
    }
    if(kEpollET){
        context_->epoll_server()->RegisterFD(fd_, this,EPOLLET|EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }else{
        context_->epoll_server()->RegisterFD(fd_, this,EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLERR);
    }
    
    if(connect(fd_,(struct sockaddr *)&origin_dst_,addr_size) == -1&& errno != EINPROGRESS){
        //connect doesn't work, are we running out of available ports ? if yes, destruct the socket
        if (errno == EAGAIN){
            CloseFd();
            return success;
        }
    }
    own_status_=STICK_CONNECTING;
    return true;
}
void CHenRightStick::Notify(StickStatus status){
    stick_status_=status;
    if(stick_status_==STICK_DISCON){
        stick_=nullptr;
    }
}
void CHenRightStick::OnEvent(int fd, basic::EpollEvent* event){
    if (event->in_events&(EPOLLERR|EPOLLRDHUP|EPOLLHUP)){
        CloseFd();
    }
    if(event->in_events&EPOLLIN){
        int n=0,last=0;
        if(kHenVerbosity){
            last=recv_bytes_;
        }
        OnReadEvent(fd);
        if(kHenVerbosity){
            n=recv_bytes_-last;
            std::cout<<FUN_LINE<<" read "<<n<<std::endl;
        }
    }
    if(event->in_events&EPOLLOUT){
        if(own_status_==STICK_CONNECTING){
            own_status_=STICK_CONNECTED;
            if(stick_){
                stick_->Notify(STICK_CONNECTED);
            }
            std::cout<<FUN_LINE<<"d connected"<<std::endl;
        }
        OnWriteEvent(fd);
    }
}
void CHenRightStick::OnShutdown(basic::EpollServer* eps, int fd){
    CloseFd();
}
void CHenBackend::CreateEndpoint(BaseContext *context,int fd){
    CHenLeftStick *stick=new CHenLeftStick(context,fd);
    UNUSED(stick);
}
PhysicalSocketServer* CHenSocketFactory::CreateSocketServer(BaseContext *context){
    std::unique_ptr<CHenBackend> backend(new CHenBackend());
    return new PhysicalSocketServer(context,std::move(backend));
}
}
