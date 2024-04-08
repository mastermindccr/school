#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

#define MAX_LEN 1024
#define CHUNK_UNIT 512
#define SEND_TIMES 14
#define SLEEP_USECOND 90

using namespace std;

int packet_count = 0;

void handler(int a){
    cout<<"packet: "<<packet_count<<'\n';
}

int main(int argc, char **argv){
    signal(SIGTERM, handler);
    struct sockaddr_in	servaddr, clientaddr;
    socklen_t           len;
	char				buff[MAX_LEN];
	time_t				ticks;
    int                 valread, connfd, file_num = atoi(argv[2]), valwrite;
    ssize_t             file_sizes[file_num];
    char*               server_addr;
    string              file_dir;
    vector<string>      file_names;
    // char*               files[file_num];
    string              path;
    
    // signal(SIGINT,  handler);
	// signal(SIGTERM, handler);
    // signal(SIGQUIT, handler);
    // strcpy(server_addr, argv[argc-1]);
    file_dir = string(argv[1]);

    for(int i = 0; i < file_num; i++){
        string file_name = string("000");
        if(i <= 9){
            file_name += "00" + to_string(i);
        }
        else if(i <= 99){
            file_name += "0" + to_string(i);
        }
        else{
            file_name += to_string(i);
        }
        file_name = file_dir + string("/") + file_name;
        file_names.push_back(file_name);
    }
	connfd = socket(AF_INET, SOCK_DGRAM, 0);

    inet_pton(AF_INET, argv[argc-1], &servaddr.sin_addr);
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(atoi(argv[argc-2]));

    // bind(listenfd, (sockaddr*) &servaddr, sizeof(servaddr));

    // No need to connect first --> use sendto/recvfrom
    // use with connect first  --> use read/write
    // connect(connfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
    // write(connfd, msg, strlen(msg))
    // read(connfd, buff, MAXLEN)
    bool error = false;
    connect(connfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    for(int i = 0; i < file_num; i++){
        char files[64*1024];
        // cout << "[INFO] Processing file #" << i << "...\n";
        path = file_names[i];
        int f = open(path.c_str(), O_RDONLY);

        file_sizes[i] = read(f, files, 64*1024);
        // if(i==0)cout << "File #" << i << " has " << file_sizes[i] << " bytes\n";

        if(file_sizes[i] < 0){
            // if(i==0)cout << "[INFO] Error occurred!\n";
            error = true;
        }
        else{
            // if(i==0)cout << "[INFO] File size " << double(file_sizes[i])/1024.0 << " KB\n";
        }
        int chunk_num = int(ceil(double(file_sizes[i])/double(CHUNK_UNIT)));
        if(chunk_num > 64) cout << "[INFO] Exceed maximum chunk number at file #" << i << " !\n";

        for(int ch = 0; ch < chunk_num; ch++){
            char chunk_buffer[CHUNK_UNIT];
            lseek(f, ch*CHUNK_UNIT, SEEK_SET);
            valread = read(f, chunk_buffer, CHUNK_UNIT);
            // if(i==0)cout << "  [INFO] Chunk #" << ch << " has size " << valread << '\n';

            uint16_t header = (i<<6) + ch;
            if(ch == chunk_num-1) header = (i<<6) + 63;
            // if(i==0)cout << hex << header << dec << '\n';
            uint32_t checksum = 0;
            // unsigned char checksum[4];
            // checksum[0] = 0, checksum[1] = 0, checksum[2] = 0, checksum[3] = 0;
            
            int cur_pos = 0;
            while(cur_pos < valread){
                // first byte
                // cout << "first byte " << hex << uint(chunk_buffer[cur_pos]) << '\n';
                checksum ^= uint(chunk_buffer[cur_pos++])<<24 & 0xffffffff;
                // cout << "Checksum: " << checksum << '\n';
                if(cur_pos >= valread) break;
                // second byte
                // cout << "second byte " << hex << uint(chunk_buffer[cur_pos]) << '\n';
                checksum ^= uint(chunk_buffer[cur_pos++]<<16) & 0xffffffff;
                // cout << "Checksum: " << checksum << '\n';
                if(cur_pos >= valread) break;
                // third byte
                checksum ^= uint(chunk_buffer[cur_pos++]<<8) & 0xffffffff;
                // cout << "Checksum: " << checksum << '\n';
                if(cur_pos >= valread) break;
                // fourth byte
                checksum ^= uint(chunk_buffer[cur_pos++]) & 0xffffffff;
                // cout << "Checksum: " << checksum << '\n';
            }
            // cout << "Checksum: " << checksum << '\n';
            // 4 byte checksum + 2 byte header + valread
            char send_buffer[4+2+valread];
            // checksum
            send_buffer[0] = checksum>>24 & 0xff;
            send_buffer[1] = checksum>>16 & 0xff;
            send_buffer[2] = checksum>>8 & 0xff;
            send_buffer[3] = checksum & 0xff;
            // cout<<"file "<<i<<' '<<ch<<'\n';
            // for(int j = 0;j<4;j++){
            //     printf("buffer %d: %x\n",j , send_buffer[j]);
            // }
            // header
            send_buffer[4] = header>>8 & 0xff;
            send_buffer[5] = header & 0xff;

            // if(i==0)printf("%X %X\n", send_buffer[4], send_buffer[5]);

            // shorthand function call
            // memcpy((void*)(send_buffer+6), (void*)chunk_buffer, valread);
            for(int k = 0; k < valread; k++){
                send_buffer[k+6] = chunk_buffer[k];
            }

            for(int k = 0; k < SEND_TIMES; k++){
                valwrite = send(connfd, send_buffer, 6+valread, 0);
                packet_count++;
                // if(i==0)cout<<"valwrite: "<<valwrite<<'\n';
                // if(valwrite < 0) if(i==0)cout << "[INFO] Trial #" << k << " failed\n";
                usleep(SLEEP_USECOND);
            }
            // if(i==0)cout << "  [INFO] Chunk #" << ch << " sent\n";
        }

        close(f);
    }
    usleep(100);
    for(int i = 0;i<100;i++)
        send(connfd, "12", 2, 0);

    recv(connfd, buff, 50, 0);
    close(connfd); // close connection
    cout<<"client packet count: "<<packet_count<<'\n';
    cout<<"client closed"<<'\n';
    return 0;
}