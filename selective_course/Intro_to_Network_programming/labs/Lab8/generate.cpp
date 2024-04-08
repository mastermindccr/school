#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sys/stat.h>
#define filenum 1000
using namespace std;

vector<char> chars;

string stringify(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = "0"+ret;
    }
    return ret;
}

int main(){
    mkdir("files", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    for(int i = 0;i<=9;i++){
        chars.push_back((char)48+i);
    }
    for(int i = 0;i<26;i++){
        chars.push_back((char)65+i);
    }
    for(int i = 0;i<26;i++){
        chars.push_back((char)97+i);
    }
    ofstream file[filenum];
    srand(time(NULL));
    for(int i = 0;i<filenum;i++){
        file[i].open("files/"+stringify(i), ios::trunc);
        for(int j = 0;j<1024*(i%10+1);j++){
            file[i]<<chars[rand()%chars.size()];
        }
        file[i].close();
    }
}