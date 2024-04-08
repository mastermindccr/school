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
#include "zlib.h"
#include <assert.h>

#define MAX_LEN 1024
#define CHUNK_UNIT 800
#define SEND_TIMES 15
#define SLEEP_USECOND 500

#define CHUNK 8192

using namespace std;

int def(FILE *source, unsigned char output_buff[], int level, ssize_t* size)
{
    int ret, flush, last_out = 0;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            for(int i = 0; i < have; i++){
                output_buff[last_out+i] = out[i];
            }
            last_out += have;
            
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    fclose(source);
    *size = last_out;
    return Z_OK;
}

int main(int argc, char **argv){
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
        unsigned char files[64*1024];
        // cout << "[INFO] Processing file #" << i << "...\n";
        path = file_names[i];
        int ret = def(fopen(file_names[i].c_str(), "r"), files, Z_BEST_COMPRESSION, &file_sizes[i]);
        if(ret != Z_OK){
            cout << "Error on compression\n";
        }
        // int f = open(file_names[i].c_str(), O_RDONLY);
        // file_sizes[i] = read(f, files, 64*1024);
        // close(f);

        int chunk_num = int(ceil(double(file_sizes[i])/double(CHUNK_UNIT)));
        if(chunk_num > 64) cout << "[INFO] Exceed maximum chunk number at file #" << i << " !\n";

        for(int ch = 0; ch < chunk_num; ch++){
            unsigned char chunk_buffer[CHUNK_UNIT];
            valread = ((ch==chunk_num-1) ? (file_sizes[i] % CHUNK_UNIT) : CHUNK_UNIT);
            if(valread == 0) valread = CHUNK_UNIT;
            for(int i = 0; i < valread; i++){
                chunk_buffer[i] = files[ch*CHUNK_UNIT+i];
            }

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
                // if(i==0)cout<<"valwrite: "<<valwrite<<'\n';
                // if(valwrite < 0) if(i==0)cout << "[INFO] Trial #" << k << " failed\n";
            }
            usleep(SLEEP_USECOND);
            // if(i==0)cout << "  [INFO] Chunk #" << ch << " sent\n";
        }
    }
    usleep(100);
    for(int i = 0;i<50;i++)
        send(connfd, "12", 2, 0);

    recv(connfd, buff, 50, 0);
    close(connfd); // close connection
    cout<<"client closed"<<'\n';
    return 0;
}