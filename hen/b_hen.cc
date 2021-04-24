#include <string>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "tcp/tcp_server.h"
#include "base/cmdline.h"
#include "base/ip_address.h"
#include "base/epoll_api.h"
#include "hen/hen_base.h"
static volatile bool g_running=true;
void signal_exit_handler(int sig)
{
    g_running=false;
}
/*          Hen
 *  	   /    \
 *   leftstick   rightstick
 */
using namespace std;
using namespace basic;
/*
./b_hen --ci 10.0.2.2
*/
int main(int argc, char *argv[]){
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);
    signal(SIGTSTP, signal_exit_handler);
    cmdline::parser a;
    a.add<string>("bi", '\0', "bind left ip", false, "0.0.0.0");
    a.add<uint16_t>("bp", '\0', "bind left port", false, 3333, cmdline::range(1, 65535));
    a.add<string>("lrp", '\0', "local right ip", false, "0.0.0.0");

    a.add<string>("ci", '\0', "c host ip", false, "0.0.0.0");
    a.add<uint16_t>("cp", '\0', "c host port", false, 3333, cmdline::range(1, 65535));

    a.parse_check(argc, argv);
    std::string bind_ip=a.get<string>("bi");
    uint16_t bind_port=a.get<uint16_t>("bp");
    std::string local_right_ip=a.get<string>("lrp");

    std::string c_host_ip=a.get<string>("ci");
    uint16_t c_host_port=a.get<uint16_t>("cp");
    {
        std::string d_host_ip("10.0.3.2");
        uint16_t d_host_port=3345;
        IpAddress ip;
        ip.FromString(d_host_ip);
        SocketAddress socket_addr(ip,d_host_port);
        sockaddr_storage d_host_storage;
        d_host_storage=socket_addr.generic_address();
        //disable_proxy_mode(d_host_storage);
        //hen_verbose(1);
    }
    sockaddr_storage b_host_right_storage;
    sockaddr_storage c_host_left_storage;
    {
        IpAddress ip;
        ip.FromString(local_right_ip);
        SocketAddress socket_addr(ip,0);
        b_host_right_storage=socket_addr.generic_address();
    }
    {
        IpAddress ip;
        ip.FromString(c_host_ip);
        SocketAddress socket_addr(ip,c_host_port);
        c_host_left_storage=socket_addr.generic_address();
    }
    IpAddress ip;
    ip.FromString(bind_ip);
    std::unique_ptr<BHenSocketFactory> socket_facotry(new BHenSocketFactory(b_host_right_storage,c_host_left_storage));
    TcpServer server(std::move(socket_facotry));
    bool success=server.Init(ip,bind_port);
    if(success){
        while(g_running){
            server.HandleEvent();
        }
    }
    return 0;
}
