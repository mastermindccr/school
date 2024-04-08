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
#include <bits/stdc++.h>
#include <zlib.h>

#define err_quit(m) { perror(m); exit(-1); }
#define CHUNK 16384

using namespace std;

///// globals ////////
int fd[1000];
int zip_fd[1000];
vector<int> files(1000);
//// end globals /////

int inf(FILE *source, FILE* dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

string stringify_zip(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = '0'+ret;
    }
	ret = '/'+ret+"_zip";
    return ret;
}

string stringify(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = '0'+ret;
    }
	ret = '/'+ret;
    return ret;
}

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

	while(1) {
		struct sockaddr_in csin;
		socklen_t csinlen = sizeof(csin);
		byte buf[20000];
		bzero(buf, 20000);
		int rlen;
		if((rlen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &csin, &csinlen)) < 0) {
			perror("recvfrom");
			break;
		}
		if(rlen < 6){
			// for(int i = 0;i<1000;i++){
			// 	close(fd[i]);
			// }
			for(int i = 0;i<100;i++)
				sendto(s, buf, rlen, 0, (struct sockaddr*) &csin, sizeof(csin));
			break;
		}
		int offset = 6;
		
		uint32_t r_checksum = ((uint32_t)buf[0]<<24)+((uint32_t)buf[1]<<16)+((uint32_t)buf[2]<<8)+(uint32_t)buf[3];
		uint32_t r_filenum = ((uint32_t)buf[4]<<2)+((uint32_t)buf[5]>>6);
		uint32_t r_chunknum = ((uint32_t)buf[5]&0x3f);
		string filename = argv[1];
		filename += '/'+stringify_zip(r_filenum);
		if(r_chunknum<files[r_filenum]) continue;
		if(!zip_fd[r_filenum]){
			zip_fd[r_filenum] = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
        }
		uint32_t checksum = 0;
		while((char)buf[offset]!='\0'){
			checksum ^= ((uint32_t)buf[offset+0]<<24)+((uint32_t)buf[offset+1]<<16)+((uint32_t)buf[offset+2]<<8)+(uint32_t)buf[offset+3];
			offset += 4;
		}
		if(checksum==r_checksum){ //checksum correct
			if(write(zip_fd[r_filenum], &buf[6], rlen-6)==-1){
				perror("failed");
			}
			files[r_filenum] = r_chunknum+1;
			if(r_chunknum==63){
				close(zip_fd[r_filenum]);
                FILE* f = fopen((string(argv[1])+"/"+stringify_zip(r_filenum)).c_str(), "r");
                string filename = argv[1];
		        filename += '/'+stringify(r_filenum);
                FILE* dest = fopen(filename.c_str(), "w+");
                inf(f, dest);
                fclose(f);
                fclose(dest);
                cout<<"filenum: "<<r_filenum<<'\n';
            }		
		}
	}
	close(s);
}
