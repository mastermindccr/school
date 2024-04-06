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
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

using namespace std;

int* inp;
int* arr;
sem_t jobs;
sem_t notifier;
sem_t jobs_mutex;
bool checkjob[15];
bool dispatched[15];

queue<pair<int, vector<int>>> job_queue;
vector<pair<int, int>> pos(15);

void job(int num, vector<int> params) {
    if(params.size()==3){ // merge
        int p = params[0];
        int q = params[1];
        int r = params[2];
        // Create L ← A[p..q] and M ← A[q+1..r]
        int n1 = q - p + 1;
        int n2 = r - q;

        int L[n1], M[n2];

        for (int i = 0; i < n1; i++)
            L[i] = arr[p + i];
        for (int j = 0; j < n2; j++)
            M[j] = arr[q + 1 + j];

        // Maintain current index of sub-arrays and main array
        int i, j, k;
        i = 0;
        j = 0;
        k = p;

        // Until we reach either end of either L or M, pick larger among
        // elements L and M and place them in the correct position at A[p..r]
        while (i < n1 && j < n2) {
            if (L[i] <= M[j]) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = M[j];
                j++;
            }
            k++;
        }

        // When we run out of elements in either L or M,
        // pick up the remaining elements and put in A[p..r]
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = M[j];
            j++;
            k++;
        }
    }
    else{ // bubble sort
        int p = params[0];
        int q = params[1];
        for(int i = p;i<q;i++){
            for(int j = i+1;j<=q;j++){
                if(arr[i]>arr[j]) {
                    swap(arr[i], arr[j]);
                }
            }
        }
    }
    checkjob[num] = true;
}

void* work(void* p){
    while(true){
        if(checkjob[14]) pthread_exit(NULL);
        sem_wait(&jobs);
        pair<int, vector<int>> tmp;
        sem_wait(&jobs_mutex);
        tmp = job_queue.front();
        job_queue.pop();
        sem_post(&jobs_mutex);
        job(tmp.first, tmp.second);
        sem_post(&notifier);
    }
}

int main(){
    ifstream ifile;
    ofstream ofiles[8];
    ifile.open("input.txt");
    int lines;
    ifile>>lines;
    int num;
    inp = (int*)malloc(lines*sizeof(int));
    arr = (int*)malloc(lines*sizeof(int));
    int cnt = 0;
    while(ifile>>num){
        inp[cnt] = num;
        cnt++;
    }
    
    int last = 0;
    for(int i = 0;i<8;i++){
        int incr = i<lines%8;
        int size = lines/8+incr;
        pos[i] = make_pair(last, last+size-1);
        last += size;
    }
    pos[8] = {pos[0].first, pos[1].second};
    pos[9] = {pos[2].first, pos[3].second};
    pos[10] = {pos[4].first, pos[5].second};
    pos[11] = {pos[6].first, pos[7].second};
    pos[12] = {pos[8].first, pos[9].second};
    pos[13] = {pos[10].first, pos[11].second};
    pos[14] = {pos[12].first, pos[13].second};

    for(int i = 1;i<=8;i++){
        vector<pthread_t> threads(8);
        // initialize array
        for(int j = 0;j<lines;j++){
            arr[j] = inp[j];
        }

        // clear queue
        queue<pair<int, vector<int>>> empty;
        swap(empty, job_queue);

        // initialize checkjob and dispatched array
        for(int j = 0;j<15;j++){
            checkjob[j] = false;
            dispatched[j] = false;
        }

        sem_init(&jobs, 1, 0);
        sem_init(&notifier, 1, 0);
        sem_init(&jobs_mutex, 1, 1);

        // there are 8 bubble sorts initially
        for(int j = 0;j<8;j++){
            vector<int> params = {pos[j].first, pos[j].second};
            job_queue.push({j, params});
        }

        for(int j = 0;j<i;j++){
            pthread_create(&threads[j], NULL, &work, NULL);
        }

        // start timer
        struct timeval start, end;
        gettimeofday(&start, 0);
        for(int j = 0;j<8;j++){
            sem_post(&jobs);
        }
        while(true){ // dispatcher
            sem_wait(&notifier);
            if(checkjob[14]){
                break;
            }
            if(checkjob[0] && checkjob[1] && !dispatched[8]){
                dispatched[8] = true;
                vector<int> params = {pos[0].first, pos[0].second, pos[1].second};
                sem_wait(&jobs_mutex);
                job_queue.push({8, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[2] && checkjob[3] && !dispatched[9]){
                dispatched[9] = true;
                vector<int> params = {pos[2].first, pos[2].second, pos[3].second};
                sem_wait(&jobs_mutex);
                job_queue.push({9, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[4] && checkjob[5] && !dispatched[10]){
                dispatched[10] = true;
                vector<int> params = {pos[4].first, pos[4].second, pos[5].second};
                sem_wait(&jobs_mutex);
                job_queue.push({10, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[6] && checkjob[7] && !dispatched[11]){
                dispatched[11] = true;
                vector<int> params = {pos[6].first, pos[6].second, pos[7].second};
                sem_wait(&jobs_mutex);
                job_queue.push({11, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[8] && checkjob[9] && !dispatched[12]){
                dispatched[12] = true;
                vector<int> params = {pos[8].first, pos[8].second, pos[9].second};
                sem_wait(&jobs_mutex);
                job_queue.push({12, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[10] && checkjob[11] && !dispatched[13]){
                dispatched[13] = true;
                vector<int> params = {pos[10].first, pos[10].second, pos[11].second};
                sem_wait(&jobs_mutex);
                job_queue.push({13, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
            if(checkjob[12] && checkjob[13] && !dispatched[14]){
                dispatched[14] = true;
                vector<int> params = {pos[12].first, pos[12].second, pos[13].second};
                sem_wait(&jobs_mutex);
                job_queue.push({14, params});
                sem_post(&jobs_mutex);
                sem_post(&jobs);
            }
        }
        //end timer
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        cout<<"worker thread #"<< i <<", elpased "<< sec*1000.0+(usec/1000.0) <<" ms\n";
        ofiles[i-1].open("output_"+to_string(i)+".txt");
        for(int l = 0;l<lines;l++){
            ofiles[i-1]<<arr[l]<<' ';
        }
        ofiles[i-1].close();
    }
}