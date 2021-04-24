#include <set>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <net/ethernet.h> /* the L2 protocols */
#include "base/base_thread.h"
namespace basic{
#define PROTO_ARP 0x0806
#define ETH2_HEADER_LEN 14
#define ETHER_HEADER_LEN sizeof(struct ether_header)
#define ETHER_ARP_LEN sizeof(struct ether_arp)
#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN
#define HW_TYPE 1
#define IPV4_LENGTH 4
#define ARP_REQUEST 0x01
#define ARP_REPLY 0x02

class OctopusArpServer:public BaseThread{
public:
    OctopusArpServer(std::set<uint32_t>& ip_set);
    ~OctopusArpServer();
    bool Init(std::string &ifname);
    void Run() override;
private:
    void CloseFd();
    void SendReply(struct in_addr *dst, struct in_addr* target);
    std::set<uint32_t> ip_set_;
    struct sockaddr_ll saddr_ll_;
    char src_mac_[ETH_ALEN];
    int fd_=-1;
};
}
