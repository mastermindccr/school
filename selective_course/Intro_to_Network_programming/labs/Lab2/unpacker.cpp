#include <bits/stdc++.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

typedef struct __attribute((packed)){
    char magic[4];
    int32_t  off_str;
    int32_t  off_cont;
    uint32_t n_files;
} Pako_header_t;

typedef struct __attribute((packed)){
    int32_t off_str;
    int32_t file_size;
    int32_t off_cont;
    uint64_t checksum;
} File_E;

int main(int argc, char** argv){
    int fd = open(argv[1], O_RDONLY);
    size_t filesize = lseek(fd, 0, SEEK_END);
    byte buf[filesize];
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, filesize);
    Pako_header_t header;
    memcpy(&header, &buf[0], sizeof(Pako_header_t));
    cout<<"There are "<<header.n_files<<" files:"<<'\n';
    vector<File_E> files(header.n_files);
    for(int i = 0;i<header.n_files;i++){
        for(int j = 0;j<2;j++){ // convert file size to big endian
            swap(buf[20+20*i+j], buf[23+20*i-j]);
        }
        for(int j = 0;j<4;j++){ // convert checksum to big endian
            swap(buf[28+20*i+j], buf[35+20*i-j]);
        }
        memcpy(&files[i], &buf[16+20*i], sizeof(File_E));
        // find file names
        int off_str = header.off_str+files[i].off_str;
        string filename;
        for(int j = 0;;j++){
            if(buf[off_str+j]==byte('\0')){ // read the null byte
                filename = string((char*) &buf[off_str], j);
                cout<<filename<<" : "<<files[i].file_size<<" bytes\n";
                break;
            }
        }
        // check check sums
        int off_cont = header.off_cont+files[i].off_cont;
        uint64_t checksum = 0;
        for(int j = 0;j<files[i].file_size;j+=8){
            uint64_t tmp = 0;
            if(j+8>files[i].file_size){ // pad zeroes
                memcpy(&tmp, &buf[off_cont+j], files[i].file_size-j);
                checksum ^= tmp;
            }
            else{
                memcpy(&tmp, &buf[off_cont+j], 8);
                checksum ^= tmp;
            }
        }
        if(checksum==files[i].checksum){ // checksum correct
            string path(argv[2]);
            ofstream new_file(path+"/"+filename, ios::out | ios::trunc | ios::binary);
            char content[files[i].file_size];
            memcpy(content, &buf[off_cont], files[i].file_size);
            new_file.write(content, files[i].file_size);
            new_file.close();
        }
    }
}