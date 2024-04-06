/*
Student No.: 109550060
Student Name: 陳星宇
Email: borachen0621@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not 
supposed to be posted to a public server, such as a 
public GitHub repository or a public web page.
*/

#include<bits/stdc++.h>
#include <sys/time.h>

using namespace std;

typedef struct{
    long long pagenum;
    int ref;
    int seq;
} LFU_page;

typedef struct{
    bool operator()(LFU_page a, LFU_page b){
        if(a.ref!=b.ref) return a.ref<b.ref;
        return a.seq<b.seq;
    }
} LFU_set_cmp;

set<LFU_page, LFU_set_cmp> LFU_set;
unordered_map<long long, pair<int,int>> LFU_map; // record ref time and seq number

unordered_map<long long, int> LRU_map; // record seq
queue<pair<long long, int>> LRU_que;

void LFU(int& seq, long long pagenum, int frame, int& hit, int& miss){
    if(!LFU_map[pagenum].first){ // page number not exist: miss
        miss++;
        if(LFU_set.size()==frame){ // page full: delete the LFU
            auto it = LFU_set.begin();
            LFU_map[(*it).pagenum] = {0, 0};
            LFU_set.erase(it);
        }
        LFU_map[pagenum].first++;
        LFU_map[pagenum].second = seq;
        LFU_set.insert({pagenum, 1, seq});
    }
    else{ // page number exists: hit
        hit++;
        LFU_set.erase({pagenum, LFU_map[pagenum].first, LFU_map[pagenum].second});
        LFU_map[pagenum].first++;
        LFU_map[pagenum].second = seq;
        LFU_set.insert({pagenum, LFU_map[pagenum].first, seq});
    }
    seq++;
}

void LRU(int& seq, int& counter, long long pagenum, int frame, int& hit, int& miss){
    if(!LRU_map[pagenum]){ // page number not exist: miss
        miss++;
        if(counter==frame){ // page full, need to delete the least
            counter--;
            while(true){
                pair<long long, int> tmp = LRU_que.front();
                LRU_que.pop();
                if(tmp.second==LRU_map[tmp.first]){ // found the last existing page number in frames
                    LRU_map[tmp.first] = 0;
                    break;
                }
            }
        }
        counter++;
    }
    else{ // page number exists: hit
        hit++;
    }
    LRU_que.push({pagenum, seq});
    LRU_map[pagenum] = seq;
    seq++;
}

int main(int argc, char* argv[]){
    int frames[4] = {64, 128, 256, 512};
    ifstream file;
    struct timeval start, end;

    printf("LFU Policy:\n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");

    // start LFU
    gettimeofday(&start, 0);

    for(int i = 0;i<4;i++){
        // initialize
        LFU_set.clear();
        LFU_map.clear();
        long long pagenum;
        int hit = 0, miss = 0;
        int seq = 0;
        file.open(argv[1]);
        while(file>>pagenum){
            LFU(seq, pagenum, frames[i], hit, miss);
        }
        printf("%d\t%d\t\t%d\t\t%.10f\n", frames[i], hit, miss, double(miss)/double(miss+hit));
        file.close();
    }
    gettimeofday(&end, 0);
    int sec = end.tv_sec - start.tv_sec;
    int usec = end.tv_usec - start.tv_usec;
    cout<<"Total elapsed time "<< sec+(usec/1000000.0) <<" sec\n\n";

    printf("LRU Policy:\n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");

    // start LRU
    gettimeofday(&start, 0);

    for(int i = 0;i<4;i++){
        // initialize
        queue<pair<long long, int>> tmp;
        LRU_que = tmp;
        LRU_map.clear();
        long long pagenum;
        int hit = 0, miss = 0, counter = 0;
        int seq = 1;
        file.open(argv[1]);
        while(file>>pagenum){
            LRU(seq, counter, pagenum, frames[i], hit, miss);
        }
        printf("%d\t%d\t\t%d\t\t%.10f\n", frames[i], hit, miss, double(miss)/double(miss+hit));
        file.close();
    }
    gettimeofday(&end, 0);
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    cout<<"Total elapsed time "<< sec+(usec/1000000.0) <<" sec\n";
}