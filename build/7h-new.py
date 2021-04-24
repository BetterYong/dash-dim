#!/usr/bin/python
#  author zsy
#  2020.12.11
#
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.link import TCLink
import time
#        ----r2-----
#    0  / 1          \0 1
#h1----r1            r4---h2---h3
#       \ 2          /2
#        ----r3------

#      2-------3
#  1---         ----6---7
#      4-------5 
bottleneckbw1=10
bottleneckbw2=10
nonbottlebw=100  
buffer_size =bottleneckbw2*1000*60*3/(1500*8) 
net = Mininet( cleanup=True )
h1 = net.addHost('h1',ip='10.0.1.1')
r1 = net.addHost('r1',ip='10.0.1.2')
r2 = net.addHost('r2',ip='10.0.2.2')
r3 = net.addHost('r3',ip='10.0.4.2')
r4 = net.addHost('r4',ip='10.0.5.2')
h2 = net.addHost('h2',ip='10.0.6.2')
h3 = net.addHost('h3',ip='10.0.7.2')
c0 = net.addController('c0')

net.addLink(h1,r1,intfName1='h1-eth0',intfName2='r1-eth0',cls=TCLink , bw=nonbottlebw, delay='10ms', max_queue_size=buffer_size*10)
net.addLink(r1,r2,intfName1='r1-eth1',intfName2='r2-eth0',cls=TCLink , bw=bottleneckbw2, delay='20ms',loss=5,max_queue_size=buffer_size)
net.addLink(r2,r4,intfName1='r2-eth1',intfName2='r4-eth0',cls=TCLink , bw=nonbottlebw, delay='10ms', max_queue_size=buffer_size*10)
net.addLink(r4,h2,intfName1='r4-eth1',intfName2='h2-eth0',cls=TCLink , bw=nonbottlebw, delay='10ms', max_queue_size=buffer_size*10)
net.addLink(h2,h3,intfName1='h2-eth1',intfName2='h3-eth0',cls=TCLink , bw=nonbottlebw, delay='10ms', max_queue_size=buffer_size*10)

net.addLink(r1,r3,intfName1='r1-eth2',intfName2='r3-eth0',cls=TCLink , bw=bottleneckbw1, delay='5ms',loss=5,max_queue_size=buffer_size)
net.addLink(r3,r4,intfName1='r3-eth1',intfName2='r4-eth2',cls=TCLink , bw=nonbottlebw, delay='5ms', max_queue_size=buffer_size*10)

net.build()

h1.cmd("ifconfig h1-eth0 10.0.1.1/24")
h1.cmd("route add default gw 10.0.1.2")

r1.cmd("ifconfig r1-eth0 10.0.1.2/24")
r1.cmd("ifconfig r1-eth1 10.0.2.1/24")
r1.cmd("ifconfig r1-eth2 10.0.4.1/24")

r1.cmd("ip route flush all proto static scope global")
r1.cmd("ip route add 10.0.2.1/24 dev r1-eth1 table 5000")
r1.cmd("ip route add default via 10.0.2.2 dev r1-eth1 table 5000")

r1.cmd("ip route add 10.0.4.1/24 dev r1-eth2 table 5001")
r1.cmd("ip route add default via 10.0.4.2 dev r1-eth2 table 5001")
r1.cmd("ip rule add from 10.0.2.1 table 5000")
r1.cmd("ip rule add from 10.0.4.1 table 5001")
r1.cmd("ip route add default gw 10.0.2.2  dev r1-eth1")

r1.cmd("ip route add to 10.0.1.0/24 via 10.0.1.1")
r1.cmd("ip route add to 10.0.2.0/24 via 10.0.2.2")
r1.cmd("ip route add to 10.0.3.0/24 via 10.0.2.2")
r1.cmd("ip route add to 10.0.6.0/24 via 10.0.2.2")
r1.cmd("ip route add to 10.0.7.0/24 via 10.0.2.2")
r1.cmd('sysctl net.ipv4.ip_forward=1')

r1.cmd("iptables -t nat -N MY_TCP")
r1.cmd("iptables -t nat -A PREROUTING -j MY_TCP")
r1.cmd("iptables -t nat -A MY_TCP -p tcp -d 10.0.7.2 -j REDIRECT --to-ports 2223")

r2.cmd("ifconfig r2-eth0 10.0.2.2/24")
r2.cmd("ifconfig r2-eth1 10.0.3.1/24")

r2.cmd("ip route add to 10.0.1.0/24 via 10.0.2.1")
r2.cmd("ip route add to 10.0.2.0/24 via 10.0.2.1")
r2.cmd("ip route add to 10.0.3.0/24 via 10.0.3.2")
r2.cmd("ip route add to 10.0.6.0/24 via 10.0.3.2")
r2.cmd("ip route add to 10.0.7.0/24 via 10.0.3.2")
r2.cmd('sysctl net.ipv4.ip_forward=1')


r3.cmd("ifconfig r3-eth0 10.0.4.2/24")
r3.cmd("ifconfig r3-eth1 10.0.5.1/24")

r3.cmd("ip route add to 10.0.1.0/24 via 10.0.4.1")
r3.cmd("ip route add to 10.0.4.0/24 via 10.0.4.1")
r3.cmd("ip route add to 10.0.5.0/24 via 10.0.5.5")
r3.cmd("ip route add to 10.0.6.0/24 via 10.0.5.2")
r3.cmd("ip route add to 10.0.7.0/24 via 10.0.5.2")
r3.cmd('sysctl net.ipv4.ip_forward=1')


r4.cmd("ifconfig r4-eth0 10.0.3.2/24")
r4.cmd("ifconfig r4-eth1 10.0.6.1/24")
r4.cmd("ifconfig r4-eth2 10.0.5.2/24")


r4.cmd("ip route flush all proto static scope global")
r4.cmd("ip route add 10.0.3.2/24 dev r4-eth0 table 5000")
r4.cmd("ip route add default via 10.0.3.1 dev r4-eth0 table 5000")

r4.cmd("ip route add 10.0.5.2/24 dev r4-eth2 table 5001")
r4.cmd("ip route add default via 10.0.4.2 dev r4-eth2 table 5001")
r4.cmd("ip rule add from 10.0.3.2 table 5000")
r4.cmd("ip rule add from 10.0.5.2 table 5001")
r4.cmd("ip route add default gw 10.0.3.1  dev r4-eth0")


r4.cmd("ip route add to 10.0.1.0/24 via 10.0.3.1")
r4.cmd("ip route add to 10.0.2.0/24 via 10.0.3.1")
r4.cmd("ip route add to 10.0.3.0/24 via 10.0.3.1")
r4.cmd("ip route add to 10.0.4.0/24 via 10.0.5.1")
r4.cmd("ip route add to 10.0.5.0/24 via 10.0.5.1")
r4.cmd("ip route add to 10.0.6.0/24 via 10.0.6.2")
r4.cmd("ip route add to 10.0.7.0/24 via 10.0.6.2")
r4.cmd('sysctl net.ipv4.ip_forward=1')


h2.cmd("ifconfig h2-eth0 10.0.6.2/24")
h2.cmd("ifconfig h2-eth1 10.0.7.1/24")

h2.cmd("ip route add to 10.0.1.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.2.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.3.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.4.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.5.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.6.0/24 via 10.0.6.1")
h2.cmd("ip route add to 10.0.7.0/24 via 10.0.7.2")
h2.cmd('sysctl net.ipv4.ip_forward=1')

h3.cmd("ifconfig h3-eth0 10.0.7.2/24")
h3.cmd("route add default gw 10.0.7.1")
net.start()
time.sleep(1)
CLI(net)
net.stop()

