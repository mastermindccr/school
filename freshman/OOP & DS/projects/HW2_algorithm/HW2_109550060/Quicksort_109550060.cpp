#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
using namespace std;

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quick_sort(vector<int> &v,int left,int right)/*index of the array to be sorted*/{
    if(left>=right) return;
    int times = left-1; //index of swap
    for(int i = left;i<=right;i++){
        if(v[i]<=v[right]/*pivot*/){
            times++;
            swap(&v[times],&v[i]);
        }
    }
        quick_sort(v,left,times-1);
        quick_sort(v,times+1,right);
}

int main() { 
    int num;
    while(cin>>num){
        if(num==0) break;
        vector<int> v(num);
        for(int i = 0;i<num;i++){
            cin>>v[i];
        }
        quick_sort(v,0,num-1);
        for(int i = 0;i<num;i++){
            cout<<v[i]<<" ";
        }
        cout<<endl;
    }
    return 0;
}
