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
#include "name_generator.h"
#define max_client 2500

using namespace std;

typedef struct {
    int fd;
    string ip;
    string port;
    string name;
    vector<string> message;
} Client;

string format(int num){
    string ori = to_string(num);
    if(ori.length()==1){
        return "0"+ori;
    }
    return ori;
}

string message(string name, string message){
    string _ret;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    _ret += format(tm.tm_year + 1900) + "-" + format(tm.tm_mon + 1)
            + "-" + format(tm.tm_mday) + " " + format(tm.tm_hour) + ":"
            + format(tm.tm_min) + ":" + format(tm.tm_sec);
    if(name == "server"){
        _ret += " *** ";
    }
    else{
        _ret += " " + name + ": ";
    }
    _ret += message;
    _ret += "\n";
    return _ret;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    srand(time(NULL));
    int sockfd, portno, newsockfd;
    Client client[max_client];
    for(int i = 0;i<max_client;i++){
        client[i].fd = 0;
    }
    fd_set readfds;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        error("setsockopt");
    }

    portno = 11111;

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,max_client);
    clilen = sizeof(cli_addr);
    while(true){
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        int max_fd = sockfd;
        for(int i = 0;i<max_client;i++){
            if(client[i].fd>0) FD_SET(client[i].fd, &readfds);
            if(client[i].fd>max_fd) max_fd = client[i].fd;
        }
        struct timeval timeout = {0, 10};
        int activity = select(max_fd+1, &readfds, NULL, NULL, &timeout);

        if(FD_ISSET(sockfd, &readfds)){ // a new client has connected
            newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");
            fcntl(newsockfd, F_SETFL, O_NONBLOCK);

            for(int i = 0;i<max_client;i++){ // initial a new client
                if(client[i].fd==0){
                    FD_SET(newsockfd, &readfds);
                    client[i].fd = newsockfd;
                    client[i].ip = inet_ntoa(cli_addr.sin_addr);
                    client[i].port = to_string(ntohs(cli_addr.sin_port));
                    client[i].name = gen(rand(), rand());
                    client[i].message.push_back(message("server", "Welcome to the simple CHAT server"));
                    
                    int online = 0;
                    for(int j = 0;j<max_client;j++){
                        if(i==j) continue;
                        if(client[j].fd>0){
                            online++;
                            client[j].message.push_back(message("server", "User " + client[i].name + " has just landed on the server"));
                        }
                    }
                    online++;
                    client[i].message.push_back(message("server", "Total " + to_string(online) + " users online now. Your name is " + client[i].name));
                    cout<<"* client connected from " + client[i].ip + ':' + client[i].port<<'\n';
                    break;
                }
            }
        }
        for(int i = 0;i<max_client;i++){
            if(client[i].fd!=0){
                char buf[20000];
                bzero(buf, 20000);
                ssize_t x = read(client[i].fd, buf, 20000);
                if(x!=-1 && x!=0){ // connected and has sent something
                    string tmp(buf);
                    tmp.pop_back();
                    if(tmp!=""){ // enter only
                        if(tmp[0]=='/'){ // command
                            stringstream ss(tmp);
                            vector<string> vec;
                            string t;
                            while(ss>>t){
                                vec.push_back(t);
                            }
                            if(vec[0]=="/name" && vec.size()!=1){ // rename
                                string name = "<";
                                for(int i = 1;i<vec.size();i++){
                                    name += vec[i];
                                    if(i!=vec.size()-1) name += ' ';
                                }
                                name += '>';
                                client[i].message.push_back(message("server", "Nickname changed to " + name));
                                for(int j = 0;j<max_client;j++){
                                    if(client[j].fd!=0 && i!=j){
                                        client[j].message.push_back(message("server", "User " + client[i].name + " renamed to " + name));
                                    }
                                }
                                client[i].name = name;
                            }
                            else if(vec[0]=="/who" && vec.size()==1){
                                string _ret = "----------------------------\n";
                                for(int j = 0;j<max_client;j++){
                                    if(client[j].fd!=0){
                                        if(i==j) _ret += "* ";
                                        else _ret += "  ";
                                        _ret += client[j].name + ' ' + client[j].ip + ':' + client[j].port+ '\n';
                                    }
                                }
                                _ret += "----------------------------\n";
                                client[i].message.push_back(_ret);
                            }
                            else{
                                client[i].message.push_back(message("server", "Unknown or incomplete command <" + vec[0] + ">"));
                            }
                        }
                        else{
                            for(int j = 0;j<max_client;j++){
                                if(client[j].fd!=0){
                                    client[j].message.push_back(message(client[i].name, tmp));
                                }
                            }
                        }
                    }
                }
                else if(x==0){ // client disconnected
                    close(client[i].fd);
                    client[i].fd = 0;
                    for(int j = 0;j<max_client;j++){
                        if(client[j].fd>0){
                            client[j].message.push_back(message("server", "User " + client[i].name + " has left the server"));
                        }
                    }
                    client[i].message.clear();
                    cout<<"* client " + client[i].ip + ':' + client[i].port + " disconnected\n";
                    continue;
                }
                

                for(auto &m : client[i].message){
                    write(client[i].fd, m.data(), m.length());
                }
                client[i].message.clear();
            }
        }
    }
    close(sockfd);
}