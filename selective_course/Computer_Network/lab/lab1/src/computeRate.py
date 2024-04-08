from scapy.config import conf
conf.ipv6_enabled = False
from scapy.all import *
import sys

# get path of pcap file
INPUTPATH1 = "../out/TCP_h4.pcap"
INPUTPATH2 = "../out/TCP_h3.pcap"
INPUTPATH3 = "../out/UDP_h4.pcap"
INPUTPATH4 = "../out/UDP_h3.pcap"
a = [INPUTPATH1,INPUTPATH2]
b = [INPUTPATH3,INPUTPATH4]

#read rcap
print("--- TCP ---")
for x in a:
    packets = rdpcap(x)
    start = packets[TCP][0].time
    end = packets[TCP][-1].time
    size = 0
    for y in packets[TCP]:
        size += len(y)
    size *= 8
    if x==INPUTPATH1:
        print("Flow1(h1->h4):",size/(end-start)/pow(2,20),"Mbps")
    else:
        print("Flow2(h2->h3):",size/(end-start)/pow(2,20),"Mbps")
print("--- UDP ---")
for x in b:
    packets = rdpcap(x)
    start = packets[UDP][0].time
    end = 0
    for y in reversed(packets[UDP]):
        if y[2].dport== 7777 :
            end = y.time
            break
    size = 0
    for y in packets[UDP]:
        size += len(y)
    size *= 8
    if x==INPUTPATH3:
        print("Flow1(h1->h4):",size/(end-start)/pow(2,20),"Mbps")
    else:
        print("Flow2(h2->h3):",size/(end-start)/pow(2,20),"Mbps")
