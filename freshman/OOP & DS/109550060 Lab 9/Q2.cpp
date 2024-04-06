#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    int num;
    int ins;
    while(cin>>num){
        if(num==0) break;
        int sum = 0;
        vector<int> v;
        for(int i = 0;i<num;i++){
            cin>>ins;
            v.push_back(ins);
        }
        for(int i = 0;i<num-1;i++){
            make_heap(v.begin()+i,v.end());
            sort_heap(v.begin()+i,v.end());
            v[i+1]+=v[i];
            sum+=v[i+1];
        }
        cout<<sum<<endl;
    }
    return 0;
}

