/*
Student No.: 109550060
Student Name: 陳星宇
Email:  borachen0621@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be 
posted to a public server, such as a public GitHub repository or a 
public web page.
*/
#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <bits/stdc++.h>
using namespace std;

struct tar_t {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

long oct_to_dec(long dec){
    long ret = 0;
    long pow = 1;
    while(dec){
        ret += dec%10*pow;
        pow*=8;
        dec/=10;
    }
    return ret;
}

void format_filename(char filename[100]){
    // format filename to preceding / and no trailing /
    bool found = false;
    for(int i = 99;i>=0;i--){
        if(!found && filename[i]!='\0'){
            if(filename[i]=='/'){ // delete trailing /
                continue;
            }
            found = true;
        }
        if(found){
            filename[i+1] = filename[i];
        }
        if(i==0){
            filename[0] = '/';
        }
    }
}

int my_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    unordered_map<string, struct tar_t*> mp;
    string str_path(path);
    if(str_path.back()!='/') str_path+='/'; // used to parse as directory
    
    FILE* file = fopen("test.tar", "r");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*) malloc(size*sizeof(char));
    fread(data, sizeof(char), size, file);
    int start = 0;

    while(start<size){
        struct tar_t* tmp = (struct tar_t*) (data+start);
        format_filename(tmp->name);
        string str_filename(tmp->name);

        if(mp[str_filename]!=nullptr && oct_to_dec(atol(tmp->mtime))>oct_to_dec(atol(mp[str_filename]->mtime))){ // this file is newer
            mp[str_filename] = tmp;
        }
        else if(mp[str_filename]==nullptr){ // this is a new file
            mp[str_filename] = tmp;
        }

        int obj_size = oct_to_dec(atol(tmp->size)); // size is in octal
        start += (obj_size/512+(obj_size%512!=0?1:0))*512 + 512;
    }
    for(auto &i: mp){
        string str_filename = i.first;
        if(str_path==str_filename.substr(0, str_path.length())){
            string substring(str_filename.begin()+str_path.length(), str_filename.end());
            if(!substring.empty() && !count(substring.begin(), substring.end(), '/')){ // is at most one depth deeper than path
                filler(buffer, substring.data(), NULL, 0);
            }
        }
    }
    
    return 0;
}

int my_getattr(const char *path, struct stat *st) {
    string str_path(path);
    if(str_path=="/"){ // root directory
        st->st_uid = 0; // root
        st->st_gid = 0; // wheel
        st->st_mode = S_IFDIR | 0444;
        return 0;
    }

    struct tar_t* ret = nullptr; // save for the newest file 

    FILE* file = fopen("test.tar", "r");
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*) malloc(file_size*sizeof(char));
    fread(data, sizeof(char), file_size, file);
    int start = 0;
    
    while(start<file_size){
        struct tar_t* tmp = (struct tar_t*) (data+start);
        format_filename(tmp->name);
        string str_filename(tmp->name);

        // file matched
        if(str_filename==str_path){
            if(ret!=nullptr && oct_to_dec(atol(tmp->mtime))>oct_to_dec(atol(ret->mtime))){ // this is a newer file
                ret = tmp;
            }
            else if(ret==nullptr){ // this is a new file
                ret = tmp;
            }
        }

        int obj_size = oct_to_dec(atol(tmp->size)); // size is in octal
        start += (obj_size/512+(obj_size%512!=0?1:0))*512 + 512;
    }
    if(ret!=nullptr){
        st->st_uid = oct_to_dec(atoi(ret->uid));
        st->st_gid = oct_to_dec(atoi(ret->gid));
        st->st_mtim.tv_sec = oct_to_dec(atol(ret->mtime));
        st->st_size = oct_to_dec(atol(ret->size)); // size is in octal
        if(atoi(ret->typeflag)==5){
            st->st_mode = S_IFDIR | oct_to_dec(atoi(ret->mode)); // mode is in octal
        }
        else{
            st->st_mode = S_IFREG | oct_to_dec(atoi(ret->mode)); // mode is in octal
        }
        return 0;
    }
    return -ENOENT;
}

int my_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    string str_path(path);
    struct tar_t* ret = nullptr;

    FILE* file = fopen("test.tar", "r");
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*) malloc(file_size*sizeof(char));
    fread(data, sizeof(char), file_size, file);

    int start = 0;
    while(start<file_size){
        struct tar_t* tmp = (struct tar_t*) (data+start);
        format_filename(tmp->name);
        string str_filename(tmp->name);
        int obj_size = oct_to_dec(atol(tmp->size)); // size is in octal

        // file matched
        if(str_filename==str_path){
            if(ret!=nullptr && oct_to_dec(atol(tmp->mtime))>oct_to_dec(atol(ret->mtime))){ // this is a newer file
                ret = tmp;
            }
            else if(ret==nullptr){ // this is a new file
                ret = tmp;
            }
        }
        start += (obj_size/512+(obj_size%512!=0?1:0))*512 + 512;
    }
    for(int i = 0;i<size;i++){
        buffer[i] = *((char*)ret+512+offset+i);
    }
    return size;
}

static struct fuse_operations op;

int main(int argc, char* argv[]){
    op.readdir = my_readdir;
    op.getattr = my_getattr;
    op.read = my_read;
    return fuse_main(argc, argv, &op, NULL);
}