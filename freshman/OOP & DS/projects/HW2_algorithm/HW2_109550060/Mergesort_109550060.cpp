#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
using namespace std;
void merge_sort(vector<int> &v,int left,int right){
    int mid = (left+right)/2;
    if(left<right){
        merge_sort(v,left,mid);
        merge_sort(v,mid+1,right);
    }
    vector<int> l(v.begin()+left,v.begin()+mid+1);
    vector<int> r(v.begin()+mid+1,v.begin()+right+1);
    l.push_back(100000);
    r.push_back(100000);
    int l_index = 0;
    int r_index = 0;
    for(int i = left;i<=right;i++){
        if(l[l_index]<=r[r_index]){
            v[i] = l[l_index];
            l_index++;
        }
        else{
            v[i] = r[r_index];
            r_index++;
        }
    }
}

int main() {
    int num;
    while(cin>>num){
        if(num==0) break;
        vector<int> v(num);
        for(int i = 0;i<num;i++){
            cin>>v[i];
        }
        merge_sort(v,0,num-1);
        for(int i = 0;i<num;i++){
            cout<<v[i]<<" ";
        }
        cout<<endl;
    }
    return 0;
}
