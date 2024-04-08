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
#include <strings.h>
#define buf_size 200000
#define sink_con 20

using namespace std;

int cmd_sockfd, sink_sockfd[sink_con];

void handler(int s) {
	string init = "/report\n";
    write(cmd_sockfd, init.data(), 8);
    char buf[buf_size];
    read(cmd_sockfd, buf, buf_size);
    string res(buf);
    cout<<res;
    close(cmd_sockfd);
    for(int i = 0;i<sink_con;i++){
        close(sink_sockfd[i]);
    }
	exit(0);
}

int main(int argc, char* argv[])
{
    signal(SIGINT,  handler);
	signal(SIGTERM, handler);
    
    struct sockaddr_in cmd_servaddr, sink_servaddr;
 
    // socket create and verification
    cmd_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cmd_sockfd == -1) {
        cout<<"socket creation failed...\n";
        exit(0);
    }
 
    // assign IP, PORT
    cmd_servaddr.sin_family = AF_INET;
    cmd_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cmd_servaddr.sin_port = htons(9998);

    for(int i = 0;i<sink_con;i++){
        sink_sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sink_sockfd[i] == -1) {
            cout<<"socket creation failed...\n";
            exit(0);
        }
    }
    
 
    // assign IP, PORT
    sink_servaddr.sin_family = AF_INET;
    sink_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sink_servaddr.sin_port = htons(9999);
 
    // connect the client socket to server socket

    if (connect(cmd_sockfd, (struct sockaddr*)&cmd_servaddr, sizeof(cmd_servaddr))!= 0){
        cout<<"connection with the server failed...\n";
        exit(0);
    }
    for(int i = 0;i<sink_con;i++){
        if (connect(sink_sockfd[i], (struct sockaddr*)&sink_servaddr, sizeof(sink_servaddr))!= 0){
            cout<<"connection with the server failed...\n";
            exit(0);
        }
    }
    
    string d;
    d.resize(buf_size);
    fill(d.begin(), d.end(), '0');
    string init = "/reset\n";
    write(cmd_sockfd, init.data(), 7);
    char buf[buf_size];
    bzero(buf, buf_size);
    read(cmd_sockfd, buf, buf_size);
    string res(buf);
    cout<<res;
    
    // function for chat
    while(true){
        for(int i = 0;i<sink_con;i++){
            write(sink_sockfd[i], d.data(), d.length());
        }
    }
}