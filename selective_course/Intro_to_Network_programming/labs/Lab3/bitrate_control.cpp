#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <sys/signal.h>

#define header_size 54

using namespace std;

static struct timeval _t0;
double rate;
static unsigned long long bytesent = 0;


double tv2s(struct timeval *ptv) {
	return 1.0*(ptv->tv_sec) + 0.000001*(ptv->tv_usec);
}

void handler(int s) {
	struct timeval _t1;
	double t0, t1;
	gettimeofday(&_t1, NULL);
	t0 = tv2s(&_t0);
	t1 = tv2s(&_t1);
    cout<<bytesent<<" bytes sent in "<<t1-t0<<"s ("<<(bytesent/1000000.0)/(t1-t0)<<" MBps)\n";
	exit(0);
}

void func(int sockfd)
{
    char buff[1000];
    bool eof = false;
    for(int i = 0;i<1000;i++){
        buff[i] = '\0';
    }
    while(size_t tmp = recv(sockfd, buff, sizeof(buff), 0)!=-1){
        for(int i = 0;i<1000;i++){
            if(buff[i]=='\0'){
                eof = true;
                break;
            }
            cout<<buff[i];
        }
        if(eof) break;
    }
    byte buffer[1200];
    struct timeval _t1;
    while(1) {
        gettimeofday(&_t1, NULL);
        double spend = (tv2s(&_t1)-tv2s(&_t0));
        if((bytesent/1000000.0)/spend<rate){
            write(sockfd, buffer, sizeof(buffer));
            bytesent+=sizeof(buffer)+header_size;
        }
	}
}
 
int main(int argc, char* argv[])
{
    rate = atof(argv[1]);
    signal(SIGINT,  handler);
	signal(SIGTERM, handler);
    int sockfd, connfd;
    struct sockaddr_in servaddr;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout<<"socket creation failed...\n";
        exit(0);
    }
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("140.113.213.213");
    servaddr.sin_port = htons(10003);
 
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!= 0){
        cout<<"connection with the server failed...\n";
        exit(0);
    }
    // function for chat
    gettimeofday(&_t0, NULL);
    func(sockfd);
    close(sockfd);
}