/*
Student No.: 109550060
Student Name: 陳星宇
Email: borachen0621@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;

int main(){
    string cmd;
    cout<<'>';
    while(getline(cin, cmd)){
        stringstream ss(cmd);
        string chunk;
        vector<char*> first;
        vector<char*> second;
        int type = 0; // 0: general, 1: input redir, 2: output redir, 3: pipe
        while(ss>>chunk){
            if(chunk=="<"){
                type = 1;
                continue;
            }
            else if(chunk==">"){
                type = 2;
                continue;
            }
            else if(chunk=="|"){
                type = 3;
                continue;
            }
            char* tmp = (char*)malloc(chunk.size()+1);
            memcpy(tmp, chunk.c_str(), chunk.size()+1);
            if(type==0){
                first.push_back(tmp);
            }
            else{
                second.push_back(tmp);
            }
        }
        pid_t pid = fork();
        if(pid==0){ // child process
            if(type==0){
                string back(first.back());
                if(back=="&"){
                    first.pop_back();
                    pid = fork(); // to prevent against zombie process
                    if(pid>0){ // child exits the process
                        exit(0);
                    }
                    else if(pid==0){ // grandchild process
                        first.push_back(NULL);
                        execvp(first[0], first.data());
                    }
                    else{
                        cout<<stderr<<'\n';
                        exit(-1);
                    }
                }
                else{
                    first.push_back(NULL);
                    execvp(first[0], first.data());
                }
            }
            else if(type==1){ // input redirection
                int fd = open(second[0], O_CREAT | O_RDONLY, S_IRWXU);
                dup2(fd, STDIN_FILENO);
                close(fd);
                first.push_back(NULL);
                execvp(first[0], first.data());
            }
            else if(type==2){ // output redirection
                int fd = open(second[0], O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU |  S_IRWXG |  S_IRWXO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
                first.push_back(NULL);
                execvp(first[0], first.data());
            }
            else{ // pipe
                int p[2];
                if(pipe(p)==-1){
                    cout<<stderr<<"\n";
                    exit(-1);
                }
                else{
                    if(fork()==0){ // child process(write)
                        close(p[0]); // close read
                        dup2(p[1], STDOUT_FILENO); // write to stdin
                        close(p[1]); // close write to turn off pipe
                        first.push_back(NULL);
                        execvp(first[0], first.data());
                    }
                    if(fork()==0){ // parent process(read)
                        close(p[1]); // close write
                        dup2(p[0], STDIN_FILENO); // read to stdout
                        close(p[0]);
                        first.push_back(NULL);
                        execvp(second[0], second.data());
                    }
                    close(p[0]);
                    close(p[1]);
                    wait(0);
                    wait(0);
                    exit(0);
                }
            }
        }
        else if(pid>0){ // parent process
            wait(0);
            cout<<'>';
        }
        else{
            cout<<stderr<<'\n';
            exit(-1);
        }
    }
}