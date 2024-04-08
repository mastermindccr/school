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
#include <regex.h>
#include "cmd.h"
#define max_client 2500

using namespace std;

typedef struct {
    int fd;
    string ip;
    string name;
    string channel;
    vector<string> message;
    bool nick;
    bool registered;
} Client;

Client client[max_client];
unordered_map<string, string> topics;
vector<string> channels;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

bool nickCollide(string nickname){
    for(auto &i: client){
        if(i.fd!=0 && i.name==nickname) return true;
    }
    return false; 
}

bool chanNotFound(string channel){
    for(auto &i: channels){
        if(channel == i) return false;
    }
    return true;
}

int main()
{
    int sockfd, portno, newsockfd;
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

    portno = 10004;

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
        struct timeval timeout = {0, 0};
        select(max_fd+1, &readfds, NULL, NULL, &timeout);

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
                    regex re("\r\n");
                    sregex_token_iterator first(tmp.begin(), tmp.end(), re, -1), last;
                    vector<string> cmds{first, last};
                    for(auto &cmd: cmds){
                        regex re2("\\s");
                        sregex_token_iterator first2(cmd.begin(), cmd.end(), re2, -1), last2;
                        vector<string> params{first2, last};
                        if(params[0]=="NICK"){
                            if(params.size()<2){
                                client[i].message.push_back(_431());
                            }
                            else if(nickCollide(params[1])){
                                client[i].message.push_back(_436(client[i].name));
                            }
                            else{
                                client[i].nick = true;
                                client[i].name = params[1];
                            }
                        }
                        else if(params[0]=="USER"){
                            if(params.size()<5){
                                client[i].message.push_back(_461(client[i].name, "USER"));
                            }
                            else if(client[i].nick){
                                client[i].registered = true;
                                client[i].message.push_back(_001(client[i].name));
                                client[i].message.push_back(_375(client[i].name));
                                client[i].message.push_back(_372(client[i].name));
                                client[i].message.push_back(_376(client[i].name));
                            }
                        }
                        else if(client[i].registered){
                            if(params[0]=="PING"){
                                if(params.size()<2){
                                    client[i].message.push_back(_409(client[i].name));
                                }
                                else{
                                    if(params.size()==2)
                                        client[i].message.push_back("PONG " + params[1] + "\r\n");
                                    else
                                        client[i].message.push_back("PONG " + params[1] + " " + params[2] + "\r\n");
                                }
                            }
                            else if(params[0]=="LIST"){
                                client[i].message.push_back(_321(client[i].name));
                                if(params.size()==1){
                                    for(auto &channel: channels){
                                        client[i].message.push_back(_322(client[i].name, channel, 1, topics[channel]));
                                    }
                                }
                                else{
                                    client[i].message.push_back(_322(client[i].name, params[1], 1, topics[params[1]]));
                                }
                                client[i].message.push_back(_323(client[i].name));
                            }
                            else if(params[0]=="JOIN"){
                                if(params.size()<2){
                                    client[i].message.push_back(_461(client[i].name, "JOIN"));
                                }
                                else{
                                    if(find(channels.begin(), channels.end(), params[1])==channels.end()) // new channel
                                        channels.push_back(params[1]);
                                    client[i].channel = params[1];
                                    client[i].message.push_back(":"+client[i].name+" JOIN :"+client[i].channel+"\r\n");
                                    if(topics[params[1]]!=""){
                                        client[i].message.push_back(_332(client[i].name, params[1], topics[params[1]]));
                                    }
                                    else{
                                        client[i].message.push_back(_331(client[i].name, params[1]));
                                    }
                                    vector<string> listUsers;
                                    for(int c = 0;c<max_client;c++){
                                        if(client[c].fd && client[c].channel==params[1]){
                                            listUsers.push_back(client[c].name);
                                        }
                                    }
                                    client[i].message.push_back(_353(client[i].name, params[1], listUsers));
                                    client[i].message.push_back(_366(client[i].name, params[1]));
                                }
                                
                            }
                            else if(params[0]=="TOPIC"){
                                if(params.size()<2){ // no enough params
                                    client[i].message.push_back(_461(client[i].name, "TOPIC"));
                                }
                                else if(chanNotFound(params[1])){ // channel not found
                                    client[i].message.push_back(_403(client[i].name, params[1]));
                                }
                                else if(params[1]!=client[i].channel){ // not on that channel
                                    client[i].message.push_back(_442(client[i].name, params[1]));
                                }
                                else{
                                    if(params.size()==2){ // view topic
                                        if(topics[params[1]]!=""){
                                            client[i].message.push_back(_332(client[i].name, params[1], topics[params[1]]));
                                        }
                                        else{
                                            client[i].message.push_back(_331(client[i].name, params[1]));
                                        }
                                    }
                                    else{
                                        string concat;
                                        params[2].erase(0, 1);
                                        for(int c = 2;c<params.size();c++){
                                            concat += params[c];
                                            if(c!=params.size()-1) concat += " ";
                                        }
                                        topics[params[1]] = concat;
                                        client[i].message.push_back(_332(client[i].name, params[1], concat));
                                    }
                                }
                            }
                            else if(params[0]=="NAMES"){
                                if(params.size()==2){
                                    vector<string> listUsers;
                                    for(int c = 0;c<max_client;c++){
                                        if(client[c].fd && client[c].channel==params[1]){
                                            listUsers.push_back(client[c].name);
                                        }
                                    }
                                    client[i].message.push_back(_353(client[i].name, params[1], listUsers));
                                    client[i].message.push_back(_366(client[i].name, params[1]));
                                }
                                else{
                                    for(auto &channel: channels){
                                        vector<string> listUsers;
                                        for(int c = 0;c<max_client;c++){
                                            if(client[c].fd && client[c].channel==channel){
                                                listUsers.push_back(client[c].name);
                                            }
                                        }
                                        client[i].message.push_back(_353(client[i].name, channel, listUsers));
                                        client[i].message.push_back(_366(client[i].name, channel));
                                    }
                                }
                            }
                            else if(params[0]=="PART"){
                                if(params.size()<2){ // not enough params
                                    client[i].message.push_back(_461(client[i].name, "JOIN"));
                                }
                                else if(chanNotFound(params[1])){ // channel not found
                                    client[i].message.push_back(_403(client[i].name, params[1]));
                                }
                                else if(client[i].channel!=params[1]){ // user not in this channel
                                    client[i].message.push_back(_442(client[i].name, params[1]));
                                }
                                else{ // part
                                    client[i].message.push_back(":"+client[i].name+" PART :"+client[i].channel+"\r\n");
                                    client[i].channel = "";
                                }
                            }
                            else if(params[0]=="USERS"){
                                client[i].message.push_back(_392(client[i].name));
                                vector<pair<string, string>> vec;
                                for(int c = 0;c<max_client;c++){
                                    if(client[c].fd){
                                        vec.push_back(make_pair(client[c].name, client[c].ip));
                                    }
                                }
                                client[i].message.push_back(_393(client[i].name, vec));
                                client[i].message.push_back(_394(client[i].name));
                            }
                            else if(params[0]=="PRIVMSG"){
                                if(params.size()==1){
                                    client[i].message.push_back(_411(client[i].name, "PRIVMSG"));
                                }
                                else if(chanNotFound(params[1])){
                                    client[i].message.push_back(_401(client[i].name, params[1]));
                                }
                                else if(client[i].channel!=params[1]){
                                    client[i].message.push_back(_442(client[i].name, params[1]));
                                }
                                else if(params.size()==2){
                                    client[i].message.push_back(_412(client[i].name));
                                }
                                else{
                                    string concat;
                                    for(int c = 2;c<params.size();c++){
                                        concat += params[c];
                                        if(c!=params.size()-1) concat += " ";
                                    }
                                    for(int c = 0;c<max_client;c++){
                                        if(client[c].fd && c!=i && client[c].channel==params[1]){
                                            client[c].message.push_back(":"+client[i].name+" PRIVMSG "+params[1]+" "+concat+"\r\n");
                                        }
                                    }
                                }
                            }
                            else if(params[0]=="QUIT"){
                                close(client[i].fd);
                                client[i] = {0, "", "", "", {}, false, false};
                                continue;
                            }
                            else{
                                client[i].message.push_back(_421(client[i].name, params[0]));
                            }
                        }
                        
                    }
                }
                // send message to the client socket

                for(auto &m : client[i].message){
                    write(client[i].fd, m.data(), m.length());
                }
                client[i].message.clear();
            }
        }
    }
    close(sockfd);
}