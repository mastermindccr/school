/*
	Raw TCP packets
*/
#include <stdio.h>	//for printf
#include <string.h> //memset
#include <sys/socket.h>	//for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // sleep()
#include <bits/stdc++.h>
#define CHUNK_SIZE 1450
#define SLEEP_TIME 1

using namespace std;

typedef struct{
    uint16_t filenum;
    char payload[CHUNK_SIZE];
} xdp;

string stringify(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = '0'+ret;
    }
	ret = '/'+ret;
    return ret;
}

unsigned short csum(unsigned short *ptr,int nbytes) 
{
	unsigned long sum;
	unsigned short oddbyte;
	unsigned short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

int main(int argc, char* argv[]){
    byte datagram[1500];
    byte receive[1500];
    bzero(datagram, 1500);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(INADDR_ANY);
    serv_addr.sin_addr.s_addr = inet_addr(argv[3]);
    socklen_t addr_len = sizeof(struct sockaddr_in);

    int s = socket(AF_INET, SOCK_RAW, 161);
    const int one = 1;
    setsockopt(s, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));
    // connect(s, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    iphdr* ip_h = (iphdr*) datagram;
    ip_h->version = 4;
    ip_h->ihl = 5;
    ip_h->tos = 0;
    ip_h->tot_len = sizeof(iphdr) + sizeof(xdp);
    ip_h->id = 0;
    ip_h->frag_off = 0;
    ip_h->ttl = 255;
    ip_h->protocol = 161;
    ip_h->check = 0;
    ip_h->saddr = inet_addr(argv[3]);
    ip_h->daddr = inet_addr(argv[3]);
    ip_h->check = csum((unsigned short *)datagram, sizeof(iphdr));
    xdp* xdp_h = (xdp*) (datagram+sizeof(iphdr));

    for(int i = 0;i<1000;i++){
        ifstream file;
        file.open(argv[1]+stringify(i));
        xdp_h->filenum = i;

        while(!file.eof()){
            bzero(xdp_h->payload, CHUNK_SIZE);
            file.read(xdp_h->payload, CHUNK_SIZE);
            sendto(s, datagram, sizeof(datagram), 0, (struct sockaddr*) &serv_addr, addr_len);
            recvfrom(s, receive, 1500, 0, (struct sockaddr*) &serv_addr, &addr_len);
            recvfrom(s, receive, 1500, 0, (struct sockaddr*) &serv_addr, &addr_len);
        }
        file.close();
    }
    close(s);
}