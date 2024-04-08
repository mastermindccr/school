#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#define max_client 50
#define buf_size 200000

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

double tv2s(struct timeval *ptv) {
	return 1.0*(ptv->tv_sec) + 0.000001*(ptv->tv_usec);
}

int main(int argc, char *argv[])
{
    struct timeval _init;
    gettimeofday(&_init, NULL);
    double init = tv2s(&_init);
    int command, sink, portno, newsockfd, cmd_client[max_client], sink_client[max_client];
    int cmd_port[max_client], sink_port[max_client];
    u_int64_t counter = 0;
    for(int i = 0;i<max_client;i++){
        cmd_client[i] = 0;
        sink_client[i] = 0;
        cmd_port[i] = sink_port[i] = 0;
    }
    fd_set cmd_readfds, sink_readfds;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in cmd_addr, sink_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    command = socket(AF_INET, SOCK_STREAM, 0);
    sink = socket(AF_INET, SOCK_STREAM, 0);

    int enable = 1;
    if (setsockopt(command, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sink, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[1]);

    cmd_addr.sin_family = AF_INET;  
    cmd_addr.sin_addr.s_addr = INADDR_ANY;  
    cmd_addr.sin_port = htons(portno);

    sink_addr.sin_family = AF_INET;  
    sink_addr.sin_addr.s_addr = INADDR_ANY;  
    sink_addr.sin_port = htons(portno+1);

    if (bind(command, (struct sockaddr *) &cmd_addr,
            sizeof(cmd_addr)) < 0) 
        error("ERROR on binding");

    if (bind(sink, (struct sockaddr *) &sink_addr,
            sizeof(cmd_addr)) < 0) 
        error("ERROR on binding");

    while(true){
        listen(command, max_client);
        listen(sink, max_client);
        clilen = sizeof(cli_addr);
        FD_ZERO(&cmd_readfds);
        FD_ZERO(&sink_readfds);
        FD_SET(command, &cmd_readfds);
        FD_SET(sink, &sink_readfds);
        int cmd_max_fd = command;
        int sink_max_fd = sink;
        struct timeval timeout = {0, 1};
        
        for(int i = 0;i<max_client;i++){
            if(cmd_client[i]>0) FD_SET(cmd_client[i], &cmd_readfds);
            if(cmd_client[i]>cmd_max_fd) cmd_max_fd = cmd_client[i];
            if(sink_client[i]>0) FD_SET(sink_client[i], &sink_readfds);
            if(sink_client[i]>sink_max_fd) sink_max_fd = sink_client[i];
        }
        select(cmd_max_fd+1, &cmd_readfds, NULL, NULL, &timeout);

        if(FD_ISSET(command, &cmd_readfds)){
            newsockfd = accept(command, 
                    (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");
            fcntl(newsockfd, F_SETFL, O_NONBLOCK);

            printf("command server: got connection from %s port %d\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            for(int i = 0;i<max_client;i++){
                if(cmd_client[i]==0){
                    FD_SET(newsockfd, &cmd_readfds);
                    cmd_client[i] = newsockfd;
                    cmd_port[i] = cli_addr.sin_port;
                    break;
                }
            }
        }

        select(sink_max_fd+1, &sink_readfds, NULL, NULL, &timeout);

        if(FD_ISSET(sink, &sink_readfds)){
            newsockfd = accept(sink, 
                    (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");
            fcntl(newsockfd, F_SETFL, O_NONBLOCK);

            printf("sink server: got connection from %s port %d\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            for(int i = 0;i<max_client;i++){
                if(sink_client[i]==0){
                    FD_SET(newsockfd, &sink_readfds);
                    sink_client[i] = newsockfd;
                    sink_port[i] = cli_addr.sin_port;
                    break;
                }
            }
        }

        for(int i = 0;i<max_client;i++){
            if(cmd_client[i]!=0){
                char read_buf[buf_size];
                bzero(read_buf, buf_size);
                ssize_t x = read(cmd_client[i], read_buf, buf_size);
                if(x!=-1 && x!=0){
                    string write_data = "";
                    string tmp(read_buf);
                    tmp.pop_back();
                    if(tmp=="/reset"){
                        struct timeval _t0;
                        gettimeofday(&_t0, NULL);
                        double t0 = tv2s(&_t0);
                        write_data += to_string(t0);
                        write_data += " RESET ";
                        write_data += to_string(counter);
                        write_data += "\n";
                        write(cmd_client[i], write_data.data(), write_data.length());
                        counter = 0;
                        init = t0;
                    }
                    else if(tmp=="/ping"){
                        struct timeval _t0;
                        gettimeofday(&_t0, NULL);
                        double t0 = tv2s(&_t0);
                        write_data += to_string(t0);
                        write_data += " PONG\n";
                        write(cmd_client[i], write_data.data(), write_data.length());
                    }
                    else if(tmp=="/report"){
                        struct timeval _t0;
                        gettimeofday(&_t0, NULL);
                        double t0 = tv2s(&_t0);
                        write_data += to_string(t0);
                        write_data += " REPORT ";
                        double interval = t0 - init;
                        write_data += to_string(interval);
                        write_data += " ";
                        write_data += to_string(8.0*counter/1000000/interval);
                        write_data += "\n";
                        write(cmd_client[i], write_data.data(), write_data.length());
                    }
                    else{ // clients
                        int cur = 0;
                        for(int i = 0;i<max_client;i++){
                            if(sink_client[i]!=0) cur++;
                        }
                        struct timeval _t0;
                        gettimeofday(&_t0, NULL);
                        double t0 = tv2s(&_t0);
                        write_data += to_string(t0);
                        write_data += " CLIENTS ";
                        write_data += to_string(cur);
                        write_data += "\n";
                        write(cmd_client[i], write_data.data(), write_data.length());
                    }
                }
                else if(x==0){
                    close(cmd_client[i]);
                    cmd_client[i] = 0;
                }
            }

            if(sink_client[i]!=0){
                char read_buf[buf_size];
                bzero(read_buf, buf_size);
                ssize_t x = read(sink_client[i], read_buf, buf_size);
                if(x!=-1 && x!=0){
                    counter += x;
                }
                else if(x==0){ // client disconnected
                    close(sink_client[i]);
                    sink_client[i] = 0;
                }
            }
        }
    }
    close(command);
    close(sink);
}