/*
https://github.com/LiamHaworth/go-tproxy
*/
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h>  //inet_addr
#include <linux/netfilter_ipv4.h>
#include <netdb.h>
#include <map>
#include "base/socket_address.h"
#define LOC __FILE__<<__LINE__<<" "
namespace tcp{
int setnonblocking(int fd);
void epoll_ctl_add(int epfd, int fd,uint32_t events);
void epoll_ctl_mod(int epfd, int fd,uint32_t events);
void epoll_ctl_del(int epfd, int fd);
int create_listen_fd(const char *ip,uint16_t port,int backlog,bool transprent);
int setnonblocking(int fd)
{
    if (-1==fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)){
        return -1;
    }
    return 0;
}
void epoll_ctl_add(int epfd, int fd,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events|EPOLLERR|EPOLLHUP;
    ev.data.fd=fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)==-1) {
        std::cout<<"epoll_ctl_add: "<<strerror(errno)<<std::endl;
        exit(1);
    }
}
void epoll_ctl_mod(int epfd, int fd,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events|EPOLLERR|EPOLLHUP;
    ev.data.fd=fd;
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev)==-1) {
        std::cout<<"epoll_ctl_mod: "<<strerror(errno)<<std::endl;
        exit(1);
    }
}
void epoll_ctl_del(int epfd, int fd){
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev)==-1) {
        std::cout<<"epoll_ctl_del: "<<strerror(errno)<<std::endl;
        exit(1);
    }
}
int create_listen_fd(const char *ip,uint16_t port,int backlog,bool transparent){
    int fd=-1;
    int yes=1;
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr =inet_addr(ip);
    servaddr.sin_port = htons(port);
    size_t addr_size = sizeof(struct sockaddr_in);
    fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0){
        return fd;
    }
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))!=0){
        close(fd);
        fd=-1;
        return fd;
    }
    if(setsockopt(fd,SOL_SOCKET, SO_REUSEPORT,&yes ,sizeof(int))!=0){
        close(fd);
        fd=-1;
        return fd;
    }
    if(bind(fd, (struct sockaddr *)&servaddr, addr_size)<0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        close(fd);
        fd=-1;
        return fd;
    }
    if(transparent&&setsockopt(fd, SOL_IP, IP_TRANSPARENT, &yes, sizeof(int))!=0){
        close(fd);
        fd=-1;
        std::cout<<"IP_TRANSPARENT failed"<<strerror(errno)<<std::endl;
        return fd;
    }
    if(-1==listen(fd,backlog)){
        close(fd);
        fd=-1;
        return fd;
    }
    return fd;
}
int bind_fake_addr(struct sockaddr *addr){
    int fd=-1;
    int yes=1;
    fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd<0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        return fd;
    }
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))!=0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        close(fd);
        fd=-1;
        return fd;
    }
    if(setsockopt(fd, SOL_IP, IP_TRANSPARENT, &yes, sizeof(int))!=0){
        std::cout<<LOC<<strerror(errno)<<std::endl;
        close(fd);
        fd=-1;
        return fd;
    }
    //struct sockaddr_in *addr_in=(struct sockaddr_in*)addr;
    //std::cout<<LOC<<inet_ntoa(addr_in->sin_addr)<<":"<<ntohs(addr_in->sin_port)<<std::endl;
    if(bind(fd, (struct sockaddr *)addr,sizeof(struct sockaddr_in))<0){
        std::cout<<LOC<<errno<<strerror(errno)<<std::endl;
        close(fd);
        fd=-1;
        return fd;
    }
   return fd;
}
}


const char *bind_ip=(const char*)"0.0.0.0";
const uint16_t bind_port=2233;
const int kMaxBacklog=128;
const int kMaxEvents=64;
const int kNumAgents=2;
const int kEpochs=4;
const int kBufferSize=1500;
const int FD_CONNECTING=0x00;
const int FD_CONNECTED=0x01;
struct FdWrap{
    FdWrap(int fd):fd(fd){}
    int fd=-1;
    FdWrap *peer=nullptr;
    int fd_status=FD_CONNECTING;
    std::string wb;
    bool operator <(FdWrap const &other) const{
        return fd<other.fd;
    }
};
void print_addr_info(sockaddr_storage &addr_storage,socklen_t addr_len){
    basic::SocketAddress addr(addr_storage);
    std::cout<<addr.ToString()<<" "<<addr_len<<std::endl;
}
static volatile bool g_running=true;
using namespace std;
using namespace tcp;
void signal_exit_handler(int sig){
    g_running=false;
}

int main(int argc, char *argv[]){
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);
    signal(SIGTSTP, signal_exit_handler);  
    char buffer[kBufferSize];
    int epfd,nfds;
    struct epoll_event events[kMaxEvents];
    std::map<int,FdWrap*> fd_db;
    int listen_fd=create_listen_fd(bind_ip,bind_port,kMaxBacklog,true);
    if(listen_fd<0){
        return -1;
    }
    epfd = epoll_create1(0);
    if(-1==epfd){
        close(listen_fd);
        return -1;
    }
    std::cout<<bind_ip<<":"<<bind_port<<std::endl;
    std::cout<<sizeof(struct sockaddr_in)<<" "<<sizeof(struct sockaddr_storage)<<std::endl;
    setnonblocking(listen_fd);
    epoll_ctl_add(epfd,listen_fd,EPOLLIN|EPOLLET);
    while(g_running){
        nfds= epoll_wait (epfd, events, kMaxEvents, 0);
        for(int i=0;i<nfds;i++){
            int fd=events[i].data.fd;
            if((events[i].events & EPOLLERR) ||
              (events[i].events & EPOLLHUP)){
                if(fd!=listen_fd){
                    FdWrap *self=nullptr;
                    FdWrap *peer=nullptr;
                    auto it=fd_db.find(fd);
                    if (it!=fd_db.end()){
                        self=it->second;
                        peer=self->peer;
                        fd_db.erase(it);
                    }
                    int fd2=-1;
                    if (peer){
                        fd2=peer->fd;
                    }
                    if (fd2>0){
                        fd_db.erase(fd2);
                        epoll_ctl_del(epfd,fd2);
                        close(fd2);
                    }
                    if(self){
                        delete self;
                    }
                    if(peer){
                        delete peer;
                    }
                    epoll_ctl_del(epfd,fd);
                    close(fd);
                }
                continue;
            }
            if(events[i].events &EPOLLIN){
                if (listen_fd==fd){
                    int left=-1;
                    sockaddr_storage addr_storage_src;
                    sockaddr_storage addr_storage_dst;
                    socklen_t addr_len = sizeof(sockaddr_storage);
                    while((left=accept(listen_fd,(sockaddr*)&addr_storage_src,&addr_len))>=0){
                        setnonblocking(left);
                        bool success=true;
                        getpeername(left,(sockaddr*)&addr_storage_src,&addr_len);
                        getsockname(left,(sockaddr*)&addr_storage_dst,&addr_len);
                        print_addr_info(addr_storage_src,addr_len);
                        print_addr_info(addr_storage_dst,addr_len);
                        int right=bind_fake_addr((sockaddr*)&addr_storage_src);
                        std::cout<<LOC<<right<<std::endl;
                        if(right<0){
                            success=false;
                        }else{
                            // for asyn connect
                            setnonblocking(right);
                        }
                        if(success){
                            epoll_ctl_add(epfd,right,EPOLLET|EPOLLIN|EPOLLOUT);
                            if(connect(right,(struct sockaddr *)&addr_storage_dst,addr_len) == -1&& errno != EINPROGRESS){
                                //connect doesn't work, are we running out of available ports ? if yes, destruct the socket
                                if(errno == EAGAIN){
                                    epoll_ctl_del(epfd,right);
                                    success=false;
                                }
                            }
                        }else{
                            success=false;
                        }
                        if(success){
                            FdWrap *client=new FdWrap(left);
                            FdWrap *server=new FdWrap(right);
                            client->peer=server;
                            server->peer=client;
                            client->fd_status=FD_CONNECTED;
                            server->fd_status=FD_CONNECTING;
                            fd_db.insert(std::make_pair(left,client));
                            fd_db.insert(std::make_pair(right,server));
                            epoll_ctl_add(epfd,left,EPOLLET|EPOLLIN);
                        }else{
                            if(right>0){
                                close(right);
                            }
                            close(left);
                        }
                    }
                }else{
                    auto it=fd_db.find(fd);
                    FdWrap *self=nullptr;
                    FdWrap *peer=nullptr;
                    if(it!=fd_db.end()){
                        self=it->second;
                        peer=self->peer;
                    }
                    while(true){
                        int off=0;
                        int n=read(fd,buffer+off,kBufferSize-off);
                        if(-1==n){
                            if(EAGAIN==errno){
                                std::cout<<fd<<" read error: "<<strerror(errno)<<std::endl;
                                continue;
                            }else{
                                epoll_ctl_del(epfd,fd);
                                close(fd);
                                if (self){fd_db.erase(it);}
                                if(peer&&peer->fd){
                                    epoll_ctl_del(epfd,peer->fd);
                                    close(peer->fd);
                                    fd_db.erase(peer->fd);
                                }
                                if(self){
                                    delete self;
                                }
                                if(peer){
                                    delete peer;
                                }
                                break;
                            }  
                        }else if(0==n){
                            std::cout<<fd<<" conection closed"<<std::endl;
                            epoll_ctl_del(epfd,fd);
                            close(fd);
                            fd_db.erase(it);
                            if(peer&&peer->fd){
                                epoll_ctl_del(epfd,peer->fd);
                                close(peer->fd);
                                fd_db.erase(peer->fd);
                            }
                            if(self){
                                delete self;
                            }
                            if(peer){
                                delete peer;
                            }
                            break;
                        }else{
                            off+=n;
                            if(peer&&peer->fd>0){
                                if(peer->fd_status==FD_CONNECTING){
                                    int old=peer->wb.size();
                                    peer->wb.resize(old+off);
                                    memcpy(&peer->wb[old],buffer,off);
                                }else{
                                    write(peer->fd,buffer,off);
                                }
                            }else{
                                std::cout<<LOC<<"where is the peer"<<std::endl;
                            }
                            break;
                        }
                    }
                }
            }
            if(events[i].events&EPOLLOUT){
                auto it=fd_db.find(fd);
                FdWrap *self=nullptr;
                if(it!=fd_db.end()){
                    self=it->second;
                    self->fd_status=FD_CONNECTED;
                    epoll_ctl_mod(epfd,fd,EPOLLET|EPOLLIN);
                    //simple test, flush all
                    if(self->wb.size()>0){
                        write(fd,self->wb.data(),self->wb.size());
                        self->wb.clear();
                    }
                }
            }
        }
    }
    std::cout<<"active: "<<fd_db.size()<<std::endl;
    while(!fd_db.empty()){
        auto it=fd_db.begin();
        FdWrap *self=it->second;
        fd_db.erase(it);
        if(self->fd>0){
            epoll_ctl_del(epfd,self->fd);
            close(self->fd);
        }
        delete self;
    }
    epoll_ctl_del(epfd,listen_fd);
    close(listen_fd);
    close(epfd);
    return 0;
}

