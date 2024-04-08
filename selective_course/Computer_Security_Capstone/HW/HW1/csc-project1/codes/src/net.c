#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <unistd.h>

#include "net.h"
#include "transport.h"
#include "esp.h"

uint16_t cal_ipv4_cksm(struct iphdr iphdr)
{
    // [TODO]: Finish IP checksum calculation
    iphdr.check = 0;
    long sum = 0;
    int len = sizeof(iphdr);
    uint16_t addr[len/2];
    memcpy(&addr[0], &iphdr, len);
    int pos = 0;
    while(len > 1)  {
        sum += addr[pos];
        pos++;
        len -= 2;
    }

    // odd octets
    if(len > 0) {
        unsigned char left[2];
        left[0] = addr[pos];
        left[1] = 0;
        sum += *(uint16_t *) left;
    }

    // compress from 32 bit into 16 bit then reverse checksum at the end
    while (sum>>16) sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

uint8_t *dissect_ip(Net *self, uint8_t *pkt, size_t pkt_len)
{
    // [TODO]: Collect information from pkt.
    // Return payload of network layer
    memcpy(&self->ip4hdr, pkt, sizeof(struct iphdr));
    self->pro = (Proto)self->ip4hdr.protocol;
    self->plen = pkt_len-self->hdrlen;
    inet_ntop(AF_INET, &self->ip4hdr.saddr, self->src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &self->ip4hdr.daddr, self->dst_ip, INET_ADDRSTRLEN);
    return pkt+self->hdrlen;
}

Net *fmt_net_rep(Net *self)
{
    // [TODO]: Fill up self->ip4hdr (prepare to send)
    self->ip4hdr.id = htons(ntohs(self->ip4hdr.id)+1);
    self->ip4hdr.tot_len = htons(sizeof(struct iphdr)+self->plen);
    self->ip4hdr.check = cal_ipv4_cksm(self->ip4hdr);
    return self;
}

void init_net(Net *self)
{
    if (!self) {
        fprintf(stderr, "Invalid arguments of %s.", __func__);
        exit(EXIT_FAILURE);
    }

    self->src_ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    self->dst_ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    self->x_src_ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    self->x_dst_ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    self->hdrlen = sizeof(struct iphdr);

    self->dissect = dissect_ip;
    self->fmt_rep = fmt_net_rep;
}
