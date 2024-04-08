#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include "dev.h"
#include "net.h"
#include "esp.h"
#include "replay.h"
#include "transport.h"

inline static int get_ifr_mtu(struct ifreq *ifr)
{
    int fd;
    if ((fd = socket(PF_PACKET, SOCK_RAW, 0)) < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, SIOCGIFMTU, ifr) < 0) {
        perror("ioctl()");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    return ifr->ifr_mtu;
}

inline static struct sockaddr_ll init_addr(struct ifreq *ifr)
{
    // [TODO]: Fill up struct sockaddr_ll addr which will be used to bind in func set_sock_fd
    struct sockaddr_ll ret = {0};
    
    int fd;
    if ((fd = socket(PF_PACKET, SOCK_RAW, 0)) < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, SIOCGIFINDEX, ifr) < 0) {
        perror("ioctl()");
        close(fd);
        exit(EXIT_FAILURE);
    }
    ret.sll_family = AF_PACKET;
    ret.sll_halen = ETHER_ADDR_LEN;
    ret.sll_ifindex = ifr->ifr_ifindex;
    ret.sll_protocol = htons(ETH_P_ALL);
    close(fd);
    return ret;
}

inline static int set_sock_fd(struct sockaddr_ll dev)
{
    int fd;

    if ((fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    bind(fd, (struct sockaddr *)&dev, sizeof(dev));
    return fd;
}

void fmt_frame(Dev *self, Net net, Esp esp, Txp txp)
{
    // [TODO]: store the whole frame into self->frame
    // and store the length of the frame into self->framelen
    int len = 0;
    // link header
    memcpy(self->frame+len, self->linkhdr, sizeof(LINKHDRLEN));
    len+=LINKHDRLEN;
    // ip header
    memcpy(self->frame+len, &net.ip4hdr, net.hdrlen);
    len+=net.hdrlen;
    // esp header
    memcpy(self->frame+len, &esp.hdr, sizeof(esp.hdr));
    len+=sizeof(esp.hdr);
    // esp payload/ txp header
    memcpy(self->frame+len, &txp.thdr, txp.hdrlen);
    len+=txp.hdrlen;
    // txp payload
    memcpy(self->frame+len, txp.pl, txp.plen);
    len+=txp.plen;
    // esp padding
    memcpy(self->frame+len, esp.pad, esp.tlr.pad_len);
    len+=esp.tlr.pad_len;
    // esp trailer
    memcpy(self->frame+len, &esp.tlr, sizeof(esp.tlr));
    len+=sizeof(esp.tlr);
    // esp auth
    memcpy(self->frame+len, esp.auth, esp.authlen);
    len+=esp.authlen;

    self->framelen = len;
}

ssize_t tx_frame(Dev *self)
{
    if (!self) {
        fprintf(stderr, "Invalid arguments of %s.", __func__);
        return -1;
    }

    ssize_t nb;
    socklen_t addrlen = sizeof(self->addr);
    nb = sendto(self->fd, self->frame, self->framelen,
                0, (struct sockaddr *)&self->addr, addrlen);

    if (nb <= 0) perror("sendto()");
    return nb;
}

ssize_t rx_frame(Dev *self)
{
    if (!self) {
        fprintf(stderr, "Invalid arguments of %s.", __func__);
        return -1;
    }

    ssize_t nb;
    socklen_t addrlen = sizeof(self->addr);

    nb = recvfrom(self->fd, self->frame, self->mtu,
                  0, (struct sockaddr *)&self->addr, &addrlen);
    if (nb <= 0)
        perror("recvfrom()");
    return nb;
}

void init_dev(Dev *self, char *dev_name)
{
    if (!self || !dev_name || strlen(dev_name) + 1 > IFNAMSIZ) {
        fprintf(stderr, "Invalid arguments of %s.", __func__);
        exit(EXIT_FAILURE);
    }

    struct ifreq ifr;
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", dev_name);

    self->mtu = get_ifr_mtu(&ifr);

    self->addr = init_addr(&ifr);
    self->fd = set_sock_fd(self->addr);

    self->frame = (uint8_t *)malloc(BUFSIZE * sizeof(uint8_t));
    self->framelen = 0;

    self->fmt_frame = fmt_frame;
    self->tx_frame = tx_frame;
    self->rx_frame = rx_frame;

    self->linkhdr = (uint8_t *)malloc(LINKHDRLEN);
}
