/*
Student No.: 109550060
Student Name: 陳星宇
Email: borachen0621@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not 
supposed to be posted to a public server, such as a 
public GitHub repository or a public web page.
*/

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdint.h>

#define max_process_num 16

using namespace std;

int dimension;

typedef struct {
    int start;
    int end;
} Range;

int main(){
    cout<<"Input The Matrix Dimension: ";
    cin>>dimension;
    cout<<'\n';
    uint32_t mat[dimension][dimension];
    for(int i = 0, num = 0;i<dimension;i++){
        for(int j = 0;j<dimension;j++, num++){
            mat[i][j] = (uint32_t)num;
        }
    }

    Range row_range[max_process_num];
    int shm_ids[dimension];
    // allocate memory addresses
    uint32_t* res_mat[dimension];
    for(int i = 0;i<dimension;i++){
        shm_ids[i] = shmget(0, dimension*sizeof(uint32_t), IPC_CREAT | 0600);
        res_mat[i] = (uint32_t*)shmat(shm_ids[i], NULL, 0);
    }
    

    for(int process = 1;process<=max_process_num;process++){
        int base_row = dimension/process;
        for(int i = 0;i<dimension;i++){
            for(int j = 0;j<dimension;j++){
                res_mat[i][j] = 0;
            }
        }
        for(int i = 0, cur_start = 0;i<process;i++){
            row_range[i] = {cur_start, cur_start+base_row-1};
            if(i<dimension-base_row*process){
                row_range[i].end++;
            }
            cur_start = row_range[i].end+1;
        }
        cout<<"Multiplying matrices using "<<process<<" process:\n";
        struct timeval start, end;
        gettimeofday(&start, 0);
        

        pid_t pid_ex = fork();
        if(pid_ex==0){ // do multiplication
            for(int cur_process = 0;cur_process<process;cur_process++){
                pid_t pid_in = fork();
                if(pid_in>0){ // parent process
                    for(int i = row_range[cur_process].start;i<=row_range[cur_process].end;i++){
                        for(int j = 0;j<dimension;j++){
                            for(int k = 0;k<dimension;k++){
                                res_mat[i][j] += mat[i][k]*mat[k][j];
                            }
                        }
                    }
                    wait(0);
                    break;
                }
                else if(pid_in==0){ // child process
                    continue;
                }
            }
            exit(0);
        }
        else if(pid_ex>0){
            wait(0);
            uint32_t res = 0;
            for(int i = 0;i<dimension;i++){
                for(int j = 0;j<dimension;j++){
                    res += res_mat[i][j];
                }
            }
            gettimeofday(&end, 0);
            int sec = end.tv_sec - start.tv_sec;
            int usec = end.tv_usec - start.tv_usec;
            cout<<"elapsed "<< sec+(usec/1000000.0) <<" sec, Checksum: "<<res<<"\n\n";
            continue;
        }
    }
    for(int i = 0;i<dimension;i++){
        shmdt(res_mat[i]);
        shmctl(shm_ids[i], IPC_RMID, NULL);
    }
    
    return 0;
}