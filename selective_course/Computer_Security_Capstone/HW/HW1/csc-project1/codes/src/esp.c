#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/pfkeyv2.h>

#include "esp.h"
#include "transport.h"
#include "hmac.h"

EspHeader esp_hdr_rec;

void get_ik(int type, uint8_t *key)
{
    // [TODO]: Dump authentication key from security association database (SADB)
    // (Ref. RFC2367 Section 2.3.4 & 2.4 & 3.1.10)
    int s = socket(PF_KEY, SOCK_RAW, PF_KEY_V2);
    if(s < 0){
        perror("socket");
        exit(1);
    }
    struct sadb_msg msg;
    memset(&msg, 0, sizeof(msg));
    msg.sadb_msg_version = PF_KEY_V2;
    msg.sadb_msg_type = SADB_DUMP;
    msg.sadb_msg_satype = type;
    msg.sadb_msg_len = sizeof(msg)/8;
    msg.sadb_msg_errno = 0;
    msg.sadb_msg_reserved = 0;
    msg.sadb_msg_seq = 0;
    msg.sadb_msg_pid = getpid();
    if(write(s, &msg, sizeof(msg))<0){
        perror("write");
        exit(1);
    }
    while(1){
        uint8_t buf[BUFSIZE];
        int size = read(s, &buf[0], BUFSIZE);
        struct sadb_msg* rec = (struct sadb_msg*) buf;
        if(rec->sadb_msg_errno != 0){
            printf("sadb err with no %d\n", rec->sadb_msg_errno);
            exit(1);
        }
        int pos = sizeof(struct sadb_msg);
        while(1){
            if(pos>=size) break;
            struct sadb_ext* ext_header = (struct sadb_ext *)(buf+pos);
            int type = ext_header->sadb_ext_type;
            
            if(type==SADB_EXT_KEY_AUTH){
                struct sadb_key* rec_key = (struct sadb_key *)(buf+pos);
                int key_size = rec_key->sadb_key_bits/8;
                memcpy(key, &buf[pos]+sizeof(struct sadb_key), key_size);
                close(s);
                return;
            }
            pos += ext_header->sadb_ext_len*8;
        }
        
        if(rec->sadb_msg_seq == 0){ // reach the end
            break;
        }
    }
}

void get_esp_key(Esp *self)
{
    get_ik(SADB_SATYPE_ESP, self->esp_key);
}

uint8_t *set_esp_pad(Esp *self)
{
    // [TODO]: Fiill up self->pad and self->pad_len (Ref. RFC4303 Section 2.4)
    int current_size = self->plen + 2; // pad-len field and next header field
    int padlen = (4-current_size%4) % 4;
    for(int i = 0;i<padlen;i++){
        self->pad[i] = i+1;
    }
    self->tlr.pad_len = padlen;
    return self->pad;
}

uint8_t *set_esp_auth(Esp *self,
                      ssize_t (*hmac)(uint8_t const *, size_t,
                                      uint8_t const *, size_t,
                                      uint8_t *))
{
    if (!self || !hmac) {
        fprintf(stderr, "Invalid arguments of %s().\n", __func__);
        return NULL;
    }

    uint8_t buff[BUFSIZE];
    size_t esp_keylen = 16;
    size_t nb = 0;  // Number of bytes to be hashed
    ssize_t ret;

    // [TODO]: Put everything needed to be authenticated into buff and add up nb
    memcpy(&buff[0], &self->hdr.spi, 4);
    memcpy(&buff[4], &self->hdr.seq, 4);
    memcpy(&buff[8], self->pl, self->plen);
    memcpy(&buff[8+self->plen], self->pad, self->tlr.pad_len);
    memcpy(&buff[8+self->plen+self->tlr.pad_len], &self->tlr.pad_len, 1);
    memcpy(&buff[9+self->plen+self->tlr.pad_len], &self->tlr.nxt, 1);
    nb = 10+self->plen+self->tlr.pad_len;
    ret = hmac(self->esp_key, esp_keylen, buff, nb, self->auth);

    if (ret == -1) {
        fprintf(stderr, "Error occurs when try to compute authentication data");
        return NULL;
    }

    self->authlen = ret;
    return self->auth;
}

uint8_t *dissect_esp(Esp *self, uint8_t *esp_pkt, size_t esp_len)
{
    // [TODO]: Collect information from esp_pkt.
    // Return payload of ESP
    memcpy(&self->hdr.spi, esp_pkt, 4);
    memcpy(&self->hdr.seq, esp_pkt+4, 4);
    uint8_t padnum = esp_pkt[esp_len-14];
    self->plen = esp_len-22-padnum;
    memcpy(self->pl, esp_pkt+8, self->plen);
    return self->pl;
}

Esp *fmt_esp_rep(Esp *self, Proto p)
{
    // [TODO]: Fill up ESP header and trailer (prepare to send)
    self->hdr.seq = htonl(ntohl(self->hdr.seq)+1);
    self->tlr.nxt = p;

    return self;
}

void init_esp(Esp *self)
{
    self->pl = (uint8_t *)malloc(MAXESPPLEN * sizeof(uint8_t));
    self->pad = (uint8_t *)malloc(MAXESPPADLEN * sizeof(uint8_t));
    self->auth = (uint8_t *)malloc(HMAC96AUTHLEN * sizeof(uint8_t));
    self->authlen = HMAC96AUTHLEN;
    self->esp_key = (uint8_t *)malloc(BUFSIZE * sizeof(uint8_t));

    self->set_padpl = set_esp_pad;
    self->set_auth = set_esp_auth;
    self->get_key = get_esp_key;
    self->dissect = dissect_esp;
    self->fmt_rep = fmt_esp_rep;
}
