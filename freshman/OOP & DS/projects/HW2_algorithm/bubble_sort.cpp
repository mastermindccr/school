#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(vector<int> &v)/*index of the array to be sorted*/{
    for(int i = 0;i<v.size();i++){
    	for(int j = 0;j<v.size()-1-i;j++){
    		if(v[j+1]>v[j]) swap(v[j+1],v[j]);
		}
	}
}

int main() { 
    int num;
    vector<double> exe_time;
    while(cin>>num){
        if(num==0) break;
        vector<int> v(num);
        for(int i = 0;i<num;i++){
            cin>>v[i];
        }
        clock_t start = clock();
        bubble_sort(v);
        clock_t end = clock();
        exe_time.push_back((double)(end-start)/CLOCKS_PER_SEC); 
    }
    for(int i = 0;i<exe_time.size();i++){
        cout<<"execution time of case "<<i+1<<" : "<<exe_time[i]<<" seconds "<<endl;
    }
    double maxtime = exe_time[0];
    double mintime = exe_time[0];
    double avgtime = exe_time[0];
    for(int i = 1;i<exe_time.size();i++){
    	if(exe_time[i]>maxtime) maxtime = exe_time[i];
    	if(exe_time[i]<mintime) mintime = exe_time[i];
    	avgtime += exe_time[i];
	}
    cout<<"maximum execution time : "<<maxtime<<" seconds "<<endl;
    cout<<"minimum execution time : "<<mintime<<" seconds "<<endl;
    cout<<"average execution time : "<<avgtime/exe_time.size()<<" seconds "<<endl;
    return 0;
}
