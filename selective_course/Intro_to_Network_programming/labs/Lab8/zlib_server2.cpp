/*
 * Lab problem set for INP course
 * by Chun-Ying Huang <chuang@cs.nctu.edu.tw>
 * License: GPLv2
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <byteswap.h>
#include <netinet/in.h>
#include <zlib.h>
#include <bits/stdc++.h>

#define err_quit(m) { perror(m); exit(-1); }
#define CHUNK 8192

using namespace std;

string stringify(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = '0'+ret;
    }
	ret = '/'+ret;
    return ret;
}

///// globals ////////
int fd[1000];
vector<int> files(1000);
//// end globals /////

uint32_t pow(uint32_t a, uint32_t b){
	uint32_t ret = 1;
	for(int i = 0;i<b;i++){
		ret*=a;
	}
	return ret;
}

int main(int argc, char *argv[]) {
	int s;
	struct sockaddr_in sin;

	if(argc < 2) {
		return -fprintf(stderr, "usage: %s ... <port>\n", argv[0]);
	}

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(strtol(argv[argc-1], NULL, 0));

	if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		err_quit("socket");

	if(bind(s, (struct sockaddr*) &sin, sizeof(sin)) < 0)
		err_quit("bind");


    // initial inflate
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    struct sockaddr_in csin;
    socklen_t csinlen = sizeof(csin);
    byte buf[1000];
	while(1) {
		bzero(buf, 1000);
		int rlen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &csin, &csinlen);
		if(rlen < 6){
			for(int i = 0;i<50;i++)
				sendto(s, buf, rlen, 0, (struct sockaddr*) &csin, sizeof(csin));
			break;
		}
		int offset = 6;
		
		uint32_t r_checksum = ((uint32_t)buf[0]<<24)+((uint32_t)buf[1]<<16)+((uint32_t)buf[2]<<8)+(uint32_t)buf[3];
		uint32_t r_filenum = ((uint32_t)buf[4]<<2)+((uint32_t)buf[5]>>6);
		uint32_t r_chunknum = ((uint32_t)buf[5]&0x3f);
		string filename = argv[1];
		filename += '/'+stringify(r_filenum);
		if(r_chunknum<files[r_filenum]) continue;
		if(!fd[r_filenum]){
			fd[r_filenum] = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
            /* allocate inflate state */
            bzero(in, CHUNK);
            bzero(out, CHUNK);
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;
            ret = inflateInit(&strm);
        }
		uint32_t checksum = 0;
		while(offset<=rlen){
			checksum ^= ((uint32_t)buf[offset+0]<<24)+((uint32_t)buf[offset+1]<<16)+((uint32_t)buf[offset+2]<<8)+(uint32_t)buf[offset+3];
			offset += 4;
		}
		if(checksum==r_checksum){ //checksum correct
            // inflate
            int last_read = 6;
            do {
                int counter = 0;
                for(int i = 0;i<CHUNK;i++){
                    if(last_read<rlen){
                        in[i] = (unsigned char)buf[last_read];
                        last_read++;
                        counter++;
                    }
                    else break;
                }
                strm.avail_in = counter;
                if (strm.avail_in == 0)
                    break;
                strm.next_in = in;

                /* run inflate() on input until output buffer not full */
                do{
                    strm.avail_out = CHUNK;
                    strm.next_out = out;
                    ret = inflate(&strm, Z_NO_FLUSH);
                        if(ret!=0 && ret!=1) cout<<r_filenum<<' '<<r_chunknum<<" ret: "<<ret<<'\n';

                    have = CHUNK - strm.avail_out;
                    write(fd[r_filenum], out, have);
                }while(strm.avail_out==0);
            }while(ret != Z_STREAM_END);

			files[r_filenum] = r_chunknum+1;
			if(r_chunknum==63){
                inflateEnd(&strm);
				close(fd[r_filenum]);
			}		
		}
	}
	close(s);
}
