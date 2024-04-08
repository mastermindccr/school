#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net.h"
#include "transport.h"

uint16_t cal_tcp_cksm(struct iphdr iphdr, struct tcphdr tcphdr, uint8_t *pl, int plen)
{
    // [TODO]: Finish TCP checksum calculation
    tcphdr.check = 0;
    long sum = 0;
    uint8_t zero = 0;
    int tot_len = 12+sizeof(tcphdr)+plen;
    uint16_t tcp_len = htons(sizeof(tcphdr) + plen);
    uint8_t tmp[tot_len];
    int pro = TCP;
    memcpy(&tmp[0], &iphdr.saddr, 4);
    memcpy(&tmp[4], &iphdr.daddr, 4);
    memcpy(&tmp[8], &zero, 1);
    memcpy(&tmp[9], &pro, 1);
    memcpy(&tmp[10], &tcp_len, 2);
    memcpy(&tmp[12], &tcphdr, sizeof(tcphdr));
    memcpy(&tmp[12+sizeof(tcphdr)], pl, plen);

    // change to uint16_t to calculate checksum
    uint16_t tmp_16[tot_len];
    memset(tmp_16, 0, sizeof(tmp_16));
    memcpy(&tmp_16[0], &tmp[0], tot_len);
    int pos = 0;
    while(tot_len > 1){
        sum += tmp_16[pos];
        pos++;
        tot_len -= 2;
    }

    // odd octets
    if(tot_len > 0) {
        unsigned char left[2];
        left[0] = tmp_16[pos];
        left[1] = 0;
        sum += *(uint16_t *) left;
    }
    // compress from 32 bit into 16 bit then reverse checksum at the end
    while (sum>>16) sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

uint8_t *dissect_tcp(Net *net, Txp *self, uint8_t *esp_payload, size_t esp_plen)
{
    // [TODO]: Collect information from segm
    // (Check IP addr & port to determine the next seq and ack value)
    // Return payload of TCP
    memcpy(&self->thdr, esp_payload, sizeof(struct tcphdr));
    self->plen = esp_plen - sizeof(struct tcphdr);

    // from client to server
    if (strcmp(net->x_src_ip, net->src_ip) == 0) {
        self->x_tx_seq = ntohl(self->thdr.seq) + self->plen;
        self->x_tx_ack = ntohl(self->thdr.ack_seq);
        self->x_src_port = ntohs(self->thdr.th_sport);
        self->x_dst_port = ntohs(self->thdr.th_dport);
    }

    // from server to client
    if (strcmp(net->x_src_ip, net->dst_ip) == 0) {
        self->x_tx_seq = ntohl(self->thdr.ack_seq);
        self->x_tx_ack = ntohl(self->thdr.seq) + self->plen;
        self->x_src_port = ntohs(self->thdr.th_dport);
        self->x_dst_port = ntohs(self->thdr.th_sport);
    }

    return esp_payload + sizeof(struct tcphdr);
}

Txp *fmt_tcp_rep(Txp *self, struct iphdr iphdr, uint8_t *data, size_t dlen)
{
    // [TODO]: Fill up self->tcphdr (prepare to send)
    self->thdr.seq = htonl(self->x_tx_seq);
    self->thdr.ack_seq = htonl(self->x_tx_ack);
    memcpy(self->pl, data, dlen);
    self->plen = dlen;
    *(((uint8_t *)&self->thdr)+13) |= (0x00001000); // set psh flag to 1
    uint8_t *tmp = malloc(sizeof(self->thdr));
    memcpy(tmp, &self->thdr, sizeof(self->thdr));
    
    self->thdr.check = cal_tcp_cksm(iphdr, self->thdr, data, dlen);

    return self;
}

inline void init_txp(Txp *self)
{
    self->pl = (uint8_t *)malloc(IP_MAXPACKET * sizeof(uint8_t));
    self->hdrlen = sizeof(struct tcphdr);

    self->dissect = dissect_tcp;
    self->fmt_rep = fmt_tcp_rep;
}

