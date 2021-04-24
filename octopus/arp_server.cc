#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netpacket/packet.h>

#include "base/ip_address.h"
#include "octopus/arp_server.h"
#include "logging/logging.h"
namespace basic{
unsigned char broadcast_haddr[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
void fill_arp_packet(struct ether_arp *arp_packet,
const unsigned char *src_mac,const unsigned char *dst_mac,
struct in_addr *src_in_addr, struct in_addr *dst_in_addr,int opcodes)
{
    arp_packet->arp_hrd = htons(ARPHRD_ETHER);
    arp_packet->arp_pro = htons(ETHERTYPE_IP);
    arp_packet->arp_hln = ETH_ALEN;
    arp_packet->arp_pln = IPV4_LENGTH;
    arp_packet->arp_op = htons(opcodes);
    memcpy(arp_packet->arp_sha, src_mac,ETH_ALEN);
    memcpy(arp_packet->arp_tha, dst_mac,ETH_ALEN);
    memcpy(arp_packet->arp_spa, src_in_addr, IPV4_LENGTH);
    memcpy(arp_packet->arp_tpa, dst_in_addr, IPV4_LENGTH);
}
OctopusArpServer::OctopusArpServer(std::set<uint32_t>& ip_set):
ip_set_(ip_set){
    memset(&saddr_ll_,0,sizeof(saddr_ll_));
}
OctopusArpServer::~OctopusArpServer(){
    CloseFd();
}
bool OctopusArpServer::Init(std::string &ifname){
    bool success=false;
    struct ifreq ifr;
    bzero(&ifr, sizeof(struct ifreq));
    memcpy(ifr.ifr_name, ifname.c_str(),ifname.size());
    int fd_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (-1==fd_) {
        DLOG(ERROR)<<strerror(errno);
        return success;
    }
    if (ioctl(fd_, SIOCGIFINDEX, &ifr) == -1){
        DLOG(ERROR)<<strerror(errno);
        CloseFd();
        return success;
    }
    saddr_ll_.sll_ifindex = ifr.ifr_ifindex;
    saddr_ll_.sll_family = PF_PACKET;
    if (ioctl(fd_, SIOCGIFADDR, &ifr) == -1){
        DLOG(ERROR)<<strerror(errno);
        CloseFd();
        return success;
    }

    if (ioctl(fd_, SIOCGIFHWADDR, &ifr)==-1){
        DLOG(ERROR)<<strerror(errno);
        CloseFd();
        return success;
    }
    memcpy(src_mac_, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    success=true;
    return success;
}
void OctopusArpServer::Run(){
    char buffer[ETHER_ARP_PACKET_LEN];
    struct ether_header  *eh=(struct ether_header *)buffer;
    struct ether_arp *arp_packet = (struct ether_arp *)(buffer+ETHER_HEADER_LEN);
	while(fd_>0&&running_){
        bzero(buffer, ETHER_ARP_PACKET_LEN);
        int r=recv(fd_,buffer,ETHER_ARP_PACKET_LEN, 0);
        if(r>0&&(ntohs(eh->ether_type)==ETHERTYPE_ARP)&&(ntohs(arp_packet->arp_op)==ARP_REQUEST)){
            eh->ether_type = htons(ETHERTYPE_ARP);
            in_addr src_in_addr=*(struct in_addr *)arp_packet->arp_spa;
            in_addr targer_in_addr =*(struct in_addr *)arp_packet->arp_tpa;
		SendReply(&src_in_addr,&targer_in_addr);
        }
	}
}
inline void OctopusArpServer::CloseFd(){
	if(fd_>0){
		close(fd_);
		fd_=-1;
	}
}
void OctopusArpServer::SendReply(struct in_addr *dst, struct in_addr* target){
    struct ether_header *eth_header;
    char buf[ETHER_ARP_PACKET_LEN];
    bzero(buf, ETHER_ARP_PACKET_LEN);
    eth_header = (struct ether_header *)buf;
    memcpy(eth_header->ether_shost, src_mac_, ETH_ALEN);
    memcpy(eth_header->ether_dhost, broadcast_haddr, ETH_ALEN);
    eth_header->ether_type = htons(ETHERTYPE_ARP);
    struct ether_arp arp_packet;
    fill_arp_packet(&arp_packet,(const unsigned char*)src_mac_,broadcast_haddr,target,dst,ARPOP_REPLY);
    memcpy(buf + ETHER_HEADER_LEN, &arp_packet, ETHER_ARP_LEN);
    sendto(fd_, buf, ETHER_ARP_PACKET_LEN, 0, (struct sockaddr *)&saddr_ll_, sizeof(struct sockaddr_ll));
}
}
