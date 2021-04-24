#include <signal.h>
#include <string.h>
#include <atomic>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "tcp/tcp_server.h"
#include "base/cmdline.h"
#include "base/ip_address.h"
#include "base/epoll_api.h"
#include "base/socket_address.h"
namespace basic{
const int kBufferSize=1500;
void print_addr_info(sockaddr_storage &addr_storage){
    basic::SocketAddress addr(addr_storage);
    std::cout<<addr.ToString()<<std::endl;
}
class EchoBackend: public Backend{
public:
    void CreateEndpoint(BaseContext *context,int fd) override;
};
class EchoSocketFactory:public SocketServerFactory{
public:
    PhysicalSocketServer* CreateSocketServer(BaseContext *context) override;
};
class EchoEndpoint:public EpollCallbackInterface{
public:
    EchoEndpoint(BaseContext *context,int fd);
    ~EchoEndpoint();
    // From EpollCallbackInterface
    void OnRegistration(basic::EpollServer* eps, int fd, int event_mask) override{}
    void OnModification(int fd, int event_mask) override {}
    void OnEvent(int fd, basic::EpollEvent* event) override;
    void OnUnregistration(int fd, bool replaced) override {}
    void OnShutdown(basic::EpollServer* eps, int fd) override;
    std::string Name() const override {return "EchoEndpoint";}
private:
    void OnWriteEvent(int fd);
    void OnReadEvent(int fd);
    void CloseFd();
    void DeleteSelf();
    BaseContext *context_;
    int fd_=-1;
    std::atomic<bool> destroyed_{false};
    int send_bytes_=0;
    int recv_bytes_=0;
};
void EchoBackend::CreateEndpoint(BaseContext *context,int fd){
    EchoEndpoint*endpoint=new EchoEndpoint(context,fd);
    UNUSED(endpoint);
}
PhysicalSocketServer* EchoSocketFactory::CreateSocketServer(BaseContext *context){
    std::unique_ptr<EchoBackend> backend(new EchoBackend());
    return new PhysicalSocketServer(context,std::move(backend));    
}
EchoEndpoint::EchoEndpoint(BaseContext *context,int fd)
:context_(context),
fd_(fd){
    sockaddr_storage addr_storage;
    socklen_t addr_len = sizeof(addr_storage);
    if(getsockname(fd,(sockaddr*)&addr_storage,&addr_len)!=-1){
        print_addr_info(addr_storage);
    }
    if(getpeername(fd,(sockaddr*)&addr_storage,&addr_len)!=-1){
        print_addr_info(addr_storage);
    }
    context_->epoll_server()->RegisterFD(fd_,this,EPOLLET|EPOLLIN|EPOLLRDHUP|EPOLLERR);
}
EchoEndpoint::~EchoEndpoint(){
    std::cout<<Name()<<" dtor"<<send_bytes_<<" "<<recv_bytes_<<std::endl;
}
void EchoEndpoint::OnWriteEvent(int fd){
    std::cout<<"EchoEndpoint::OnWriteEvent should not be called"<<std::endl;
}
void EchoEndpoint::OnReadEvent(int fd){
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
            //std::cout<<"read "<<n<<std::endl;
            int w=send(fd_,buffer,n,0);
            send_bytes_+=w;
        }
    }
}
void EchoEndpoint::CloseFd(){
    if(fd_>0){
        context_->epoll_server()->UnregisterFD(fd_);
        close(fd_);
        fd_=-1;
    }
    DeleteSelf();    
}
void EchoEndpoint::DeleteSelf(){
    if(destroyed_){
        return;
    }
    destroyed_=true;
    context_->PostTask([this]{
        delete this;
    });   
}
void EchoEndpoint::OnEvent(int fd, basic::EpollEvent* event){
    if(event->in_events & EPOLLIN){
        OnReadEvent(fd);
    }
    if(event->in_events&EPOLLOUT){
        OnWriteEvent(fd);
    }
}
void EchoEndpoint::OnShutdown(basic::EpollServer* eps, int fd){
    CloseFd();
}
}
using namespace basic;
using namespace std;
static volatile bool g_running=true;
void signal_exit_handler(int sig)
{
    g_running=false;
}
/*
./d_echo
*/
int main(int argc, char *argv[]){
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);
    signal(SIGTSTP, signal_exit_handler);    
    cmdline::parser a;
    a.add<string>("bi", '\0', "bind ip", false, "0.0.0.0");
    a.add<uint16_t>("bp", '\0', "bind port", false, 3345, cmdline::range(1, 65535));
    a.parse_check(argc, argv); 
    std::string host=a.get<string>("bi");
    uint16_t port=a.get<uint16_t>("bp");
    IpAddress ip;
    ip.FromString(host);
    std::cout<<host<<" "<<port<<std::endl;
    std::unique_ptr<EchoSocketFactory> socket_facotry(new EchoSocketFactory);
    TcpServer server(std::move(socket_facotry));
    bool success=server.Init(ip,port);
    if(success){
        while(g_running){
            server.HandleEvent();
        }
    }
    return 0;
}
