#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int num;
    int tp;
    cin>>num;
    vector<int> v(19,-1);
    vector<int> u(19,-1);
    for(int i = 0;i<num;i++){
        cin>>tp;
        int r = tp%19;
        int l = r;
        for(int j = 0;v[r]!=-1;j++){
            r+=1;
            while(r>=19){
            r%=19;
            }
        }
        v[r] = tp;
        for(int j = 0;u[l]!=-1;j++){
            l+=(2*j+1);
            while(l>=19){
                l%=19;
            }
        }
        u[l] = tp;
    }
    for(int i = 0;i<19;i++){
        if(v[i]==-1) cout<<0<<" ";
        else cout<<v[i]<<" ";
    }
    cout<<endl;
    for(int i = 0;i<19;i++){
        if(u[i]==-1) cout<<0<<" ";
        else cout<<u[i]<<" ";
    }
    return 0;
}
