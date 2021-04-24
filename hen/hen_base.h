#include <stdint.h>
#include <string>
#include <atomic>
#include "base/epoll_api.h"
#include "base/socket_address.h"
#include "tcp/tcp_server.h"
namespace basic{
enum StickStatus:uint8_t{
    STICK_MIN,
    STICK_CONNECTING,
    STICK_CONNECTED,
    STICK_DISCON,
    STICK_MAX,
};
void hen_verbose(int v);
void disable_proxy_mode(sockaddr_storage addr);
void enable_epoll_et();
class HenBase{
public:
    HenBase(BaseContext *context,int fd);
    virtual ~HenBase(){}
    virtual void Notify(StickStatus status){}
    void set_stick(HenBase *stick){stick_=stick;}
    void Sink(const char *pv,int sz);
    virtual std::string Name() const=0;
protected:
    void FlushBuffer();
    void OnWriteEvent(int fd);
    virtual void OnReadEvent(int fd);
    void CheckCloseFd();
    void CloseFd();
    void DeleteSelf();
    BaseContext *context_=nullptr;
    int fd_=-1;
    std::atomic<bool> destroyed_{false};
    HenBase *stick_=nullptr;
    StickStatus own_status_=STICK_MIN;
    StickStatus stick_status_=STICK_MIN;
    std::string wb_;
    std::string rb_;
    int send_bytes_=0;
    int recv_bytes_=0;
};
/*          BHen
 *  	   /    \
 *   leftstick   rightstick
 */
class BHenLeftStick:public HenBase,
public EpollCallbackInterface{
public:
    BHenLeftStick(BaseContext *context,int fd,
    		const sockaddr_storage &b_hen_addr,
			const sockaddr_storage &c_hen_addr);
    ~BHenLeftStick();
    void Notify(StickStatus status) override;
    // From EpollCallbackInterface
    void OnRegistration(basic::EpollServer* eps, int fd, int event_mask) override{}
    void OnModification(int fd, int event_mask) override {}
    void OnEvent(int fd, basic::EpollEvent* event) override;
    void OnUnregistration(int fd, bool replaced) override {}
    void OnShutdown(basic::EpollServer* eps, int fd) override;
    std::string Name() const override {return "BHenLeftStick";}
};
class BHenRightStick:public HenBase,
public EpollCallbackInterface{
public:
    BHenRightStick(BaseContext *context,int fd,
    		 const sockaddr_storage &b_hen_addr,
			 const sockaddr_storage &c_hen_addr,
			 const sockaddr_storage &origin_dst);
    ~BHenRightStick();
    bool AsynConnect();
    void Notify(StickStatus status) override;
    // From EpollCallbackInterface
    void OnRegistration(basic::EpollServer* eps, int fd, int event_mask) override{}
    void OnModification(int fd, int event_mask) override {}
    void OnEvent(int fd, basic::EpollEvent* event) override;
    void OnUnregistration(int fd, bool replaced) override {}
    void OnShutdown(basic::EpollServer* eps, int fd) override;
    std::string Name() const override {return "BHenRightStick";}
private:
	const sockaddr_storage &b_hen_addr_;
	const sockaddr_storage &c_hen_addr_;
};
class BHenBackend: public Backend{
public:
	BHenBackend(const sockaddr_storage &b_hen_addr,
			const sockaddr_storage &c_hen_addr)
			:b_hen_addr_(b_hen_addr),
			 c_hen_addr_(c_hen_addr){}
    void CreateEndpoint(BaseContext *context,int fd) override;
    const sockaddr_storage &b_hen_addr_;
	const sockaddr_storage &c_hen_addr_;
};
class BHenSocketFactory:public SocketServerFactory{
public:
	BHenSocketFactory(sockaddr_storage &b_hen_addr,sockaddr_storage &c_hen_addr)
	:b_hen_addr_(b_hen_addr),c_hen_addr_(c_hen_addr){}
    PhysicalSocketServer* CreateSocketServer(BaseContext *context) override;
private:
    sockaddr_storage b_hen_addr_;
	sockaddr_storage c_hen_addr_;
};
/*        CHen
 *      /      \
 *   leftstick right_stick
 */
class CHenLeftStick:public HenBase,
public EpollCallbackInterface{
public:
	CHenLeftStick(BaseContext *context,int fd);
	~CHenLeftStick();
    void Notify(StickStatus status) override;
    void OnReadEvent(int fd) override;
    // From EpollCallbackInterface
    void OnRegistration(basic::EpollServer* eps, int fd, int event_mask) override{}
    void OnModification(int fd, int event_mask) override {}
    void OnEvent(int fd, basic::EpollEvent* event) override;
    void OnUnregistration(int fd, bool replaced) override {}
    void OnShutdown(basic::EpollServer* eps, int fd) override;
    std::string Name() const override {return "CHenLeftStick";}
private:
    void CreateRightStick();
    bool meta_parsed_=false;
    std::string meta_buf_;
};
class CHenRightStick:public HenBase,
public EpollCallbackInterface{
public:
	CHenRightStick(BaseContext *context,int fd,
			  sockaddr_storage &origin_dst);
    ~CHenRightStick();
    bool AsynConnect();
    void Notify(StickStatus status) override;
    // From EpollCallbackInterface
    void OnRegistration(basic::EpollServer* eps, int fd, int event_mask) override{}
    void OnModification(int fd, int event_mask) override {}
    void OnEvent(int fd, basic::EpollEvent* event) override;
    void OnUnregistration(int fd, bool replaced) override {}
    void OnShutdown(basic::EpollServer* eps, int fd) override;
    std::string Name() const override {return "CHenRightStick";}
private:
    sockaddr_storage origin_dst_;
};
class CHenBackend: public Backend{
public:
	CHenBackend(){}
    void CreateEndpoint(BaseContext *context,int fd) override;
};
class CHenSocketFactory:public SocketServerFactory{
public:
	CHenSocketFactory(){}
    PhysicalSocketServer* CreateSocketServer(BaseContext *context) override;
};
}
