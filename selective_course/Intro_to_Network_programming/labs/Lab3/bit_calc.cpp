#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <iostream>
#include <string>

using namespace std;

void func(int sockfd)
{
    char buff[1000];
    bool eof = false;
    for(int i = 0;i<1000;i++){
        buff[i] = '\0';
    }
    while(recv(sockfd, buff, sizeof(buff), 0)!=-1){
        for(int i = 0;i<1000;i++){
            if(buff[i]=='\0'){
                eof = true;
                break;
            }
            cout<<buff[i];
        }
        if(eof) break;
    }
    char go[sizeof("GO\n")] = "GO\n";
    write(sockfd, go, 3);
    
    uint32_t bytes = 0;
    int newline = 0;
    eof = false;
    for(int i = 0;i<1000;i++){
        buff[i] = '\0';
    }

    while(recv(sockfd, buff, sizeof(buff), 0)!=-1){
        for(int i = 0;i<1000;i++){
            if(buff[i]=='\0' && newline>=4){
                eof = true;
                break;
            }
            if(buff[i]=='\n'){
                newline++;
            }
            else if(newline==1 && (isdigit(buff[i]) || isalpha(buff[i]))){
                bytes++;
            }
            buff[i] = '\0';
        }
        if(eof) break;
    }

    uint32_t data = htonl(bytes);
    char data2[20];
    for(int i = 0;i<20;i++){
        data2[i] = '\0';
    }
    sprintf(data2, "%d\n", bytes);
    int w;
    for(w = 0;w<20;w++){
        if(data2[w]=='\0') break;
    }
    write(sockfd, data2, w);
    for(int i = 0;i<1000;i++){
        buff[i] = '\0';
    }
    while(recv(sockfd, buff, sizeof(buff), 0)!=-1){
        for(int i = 0;i<1000;i++){
            if(buff[i]=='\0'){
                eof = true;
                break;
            }
            cout<<buff[i];
        }
        if(eof) break;
    }
}
 
int main()
{
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
    servaddr.sin_port = htons(10002);
 
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!= 0){
        cout<<"connection with the server failed...\n";
        exit(0);
    }
    // function for chat
    func(sockfd);
 
    // close the socket
    close(sockfd);
}