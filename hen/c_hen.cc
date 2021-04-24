#include <string>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include "tcp/tcp_server.h"
#include "base/cmdline.h"
#include "hen/hen_base.h"
static volatile bool g_running=true;
void signal_exit_handler(int sig)
{
    g_running=false;
}
using namespace std;
using namespace basic;
/*
./c_hen
*/
int main(int argc, char *argv[]){
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);
    signal(SIGTSTP, signal_exit_handler);
    cmdline::parser a;
    a.add<string>("bi", '\0', "bind ip", false, "0.0.0.0");
    a.add<uint16_t>("bp", '\0', "bind port", false, 3333, cmdline::range(1, 65535));
    a.parse_check(argc, argv);
    std::string bind_ip=a.get<string>("bi");
    uint16_t bind_port=a.get<uint16_t>("bp");
    //hen_verbose(1);
    IpAddress ip;
    ip.FromString(bind_ip);
    std::unique_ptr<CHenSocketFactory> socket_facotry(new CHenSocketFactory());
    TcpServer server(std::move(socket_facotry));
    bool success=server.Init(ip,bind_port);
    if(success){
        while(g_running){
            server.HandleEvent();
        }
    }
    return 0;
}
