#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <regex>
#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

vector<int> get_time(int sockfd)
{
    char buf[1000];
    bzero(buf, 1000);
    recv(sockfd, buf, sizeof(buf), 0);
    string s(buf);
    regex r("^(\\d+):(\\d+):(\\d+)");
    smatch match;
    if (regex_search(s, match, r))
        return {stoi(match[1].str()), stoi(match[2].str()), stoi(match[3].str())};
    return {};
}
 
int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout<<"socket creation failed...\n";
        exit(0);
    }
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
 
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!= 0){
        cout<<"connection with the server failed...\n";
        exit(0);
    }
    // get hour, minute and second
    vector<int> hms = get_time(sockfd);

    // get local time and change it to the specified time
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    tm_info->tm_hour = hms[0];
    tm_info->tm_min = hms[1];
    tm_info->tm_sec = hms[2];
    time_t new_time = mktime(tm_info);

    // calculate the secret
    srand((uint32_t)new_time);
    uint32_t ans = rand();

    // the answer has to be the number in string type
    string send_ans = to_string(ans);

    // send secret to the server
    write(sockfd, &send_ans[0], sizeof(send_ans));

    // get the returned output
    char buf[1000];
    bzero(buf, 1000);
    recv(sockfd, buf, sizeof(buf), 0);
    string s(buf);
    cout<<s;

    // close the socket
    close(sockfd);
}