#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>
#define max_client 10

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, newsockfd, client[max_client];
    for(int i = 0;i<max_client;i++){
        client[i] = 0;
    }
    fd_set readfds;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    vector<char*> args;
    for(int i = 2;i<argc;i++){
        args.push_back(argv[i]);
    }

    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    while(true){
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        int max_fd = sockfd;
        for(int i = 0;i<max_client;i++){
            if(client[i]>0) FD_SET(client[i], &readfds);
            if(client[i]>max_fd) max_fd = client[i];
        }
        int activity = select(max_fd+1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &readfds)){
            newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");

            printf("server: got connection from %s port %d\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            for(int i = 0;i<max_client;i++){
                if(client[i]==0){
                    FD_SET(newsockfd, &readfds);
                    client[i] = newsockfd;
                    break;
                }
            }
        }

        for(int i = 0;i<max_client;i++){
            if(FD_ISSET(client[i], &readfds) && client[i]!=0){
                pid_t pid = fork();
                if(pid==0){ // child process
                    pid_t pid = fork();
                    if(pid==0){
                        int stderr = dup(STDERR_FILENO);
                        dup2(client[i], STDOUT_FILENO);
                        dup2(client[i], STDIN_FILENO);
                        dup2(client[i], STDERR_FILENO);
                        args.push_back(NULL);
                        if (execvp(args[0], args.data())==-1) {
                            dup2(stderr, STDERR_FILENO);
                            perror("execvp");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else{
                        exit(0);
                    }                   
                }
                else if(pid>0){ // parent process
                    wait(0);
                    close(client[i]);
                    client[i] = 0;
                }
                else{
                    cout<<stderr<<'\n';
                    exit(-1);
                }
            }
        }
    }
    close(sockfd);
}