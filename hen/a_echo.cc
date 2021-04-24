#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <memory.h>
#include <chrono>
#include <errno.h>   // for errno and strerror_r
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h>  //inet_addr  
#include <netinet/tcp.h> //TCP_NODELAY
#include "base/cmdline.h"
#define LOC __FILE__<<__LINE__<<" "
inline int64_t WallTimeNowInUsec(){
    std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();    
    std::chrono::microseconds mic = std::chrono::duration_cast<std::chrono::microseconds>(d);
    return mic.count(); 
}
inline int64_t TimeMillis(){
    return WallTimeNowInUsec()/1000;
}
const int kBufferSize=1500;
class TcpClient{
public:
    TcpClient(int uuid,int many);
    ~TcpClient();
    bool Init(const char *ip,uint16_t port,const char *fakeip=nullptr,uint16_t fake_port=0);
    bool Loop();
private:
    int SendMessage();
    int WaitReply();
    void CloseFd();
    int uuid_=-1;
    int many_=0;
    int mid_=0;
    int sockfd_=-1;
};
TcpClient::TcpClient(int uuid,int many):uuid_(uuid),many_(many){}
TcpClient::~TcpClient(){
    CloseFd();
    std::cout<<"dtor"<<std::endl;
}
bool TcpClient::Init(const char *ip,uint16_t port,const char *fakeip,uint16_t fake_port){
    bool success=true;
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
    int flag = 1,yes=1; 
    if ((sockfd_= socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cout<<"Error : Could not create socket"<<std::endl;
        success=false;
        return success;
    }
    setsockopt (sockfd_, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
    if(fakeip){
        struct sockaddr_in fakeaddr;
        fakeaddr.sin_family = AF_INET;
        fakeaddr.sin_addr.s_addr =inet_addr(fakeip);
        fakeaddr.sin_port = htons(fake_port);
    if(setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))!=0){
        CloseFd();
        success=false;
        return success;
    }
    if(setsockopt(sockfd_, SOL_IP, IP_TRANSPARENT, &yes, sizeof(int))!=0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        CloseFd();
        success=false;
        return success;
    }
    if(bind(sockfd_, (struct sockaddr *)&fakeaddr, sizeof(fakeaddr))<0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        CloseFd();
        success=false;
        return success;
    }
    }
    if (connect(sockfd_,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0) { 
        std::cout<<"connection with the server failed"<<std::endl; 
        CloseFd();
        success=false;
        return success;
    }
    return success;
}
bool TcpClient::Loop(){
    bool done=true;
    if(mid_<many_&&sockfd_>0){
        int sent=SendMessage();
        if(sent>0&&WaitReply()>0){
            done=false;
        }
        if(sent>0){
            usleep(10000);
        }
    }
    return done;
}
int TcpClient::SendMessage(){
    char buffer[kBufferSize];
    std::string message=std::string("helle world")+std::to_string(mid_);
    memcpy(buffer,message.data(),message.size());
    mid_++;
    return write(sockfd_,buffer,message.size());
}
int TcpClient::WaitReply(){
    char buffer[kBufferSize]={0};
    int ret=recv(sockfd_, buffer, kBufferSize, 0);
    if(ret>0){
        std::string messsage(buffer,ret);
        std::cout<<"echo from server "<<buffer<<std::endl;
    }
    return ret;
}
void TcpClient::CloseFd(){
    if(sockfd_>0){
        close(sockfd_);
        sockfd_=-1;
    }
}
static volatile bool g_running=true;
void signal_exit_handler(int sig)
{
    g_running=false;
}
/*
./a_echo --di 10.0.3.2
*/
using namespace std;
int main(int argc, char *argv[]){
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);
    signal(SIGHUP, signal_exit_handler);//ctrl+c
    signal(SIGTSTP, signal_exit_handler);//ctrl+z   
    cmdline::parser a;
    a.add<string>("di", '\0', "dest ip", false, "0.0.0.0");
    a.add<uint16_t>("dp", '\0', "dest port", false, 3345, cmdline::range(1, 65535));
    a.parse_check(argc, argv); 
    std::string server_ip=a.get<string>("di");
    uint16_t server_port=a.get<uint16_t>("dp");
    const char *fake_ip="10.0.1.1";
    uint16_t fake_port=4456;
    std::cout<<server_ip<<":"<<server_port<<std::endl;
    int uuid=0;
    int many=10;
    TcpClient client(uuid,many);
    int32_t last=TimeMillis();
    if(client.Init(server_ip.c_str(),server_port,fake_ip,fake_port)){
        while(!client.Loop()&&g_running){}
    }
    int32_t delta=TimeMillis()-last;
    std::cout<<"run time:"<<delta<<std::endl;
    return 0;
}
