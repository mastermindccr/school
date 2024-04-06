#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int num;
vector<vector<int>> edge;
vector<bool> traversed;

vector<int> traverse(vector<int>v){
    for(int i = 0;i<num;i++){
        if(!traversed[i] && (v.size() == 0 || edge[v[v.size()-1]][i] == 1)){
            traversed[i] = true;
            v.push_back(i);
            return traverse(v);
        } 
    }
    return v;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    cin>>num;
    edge.resize(num,vector<int>(num));
    traversed.resize(num);
    for(int i = 0;i<num;i++){
        traversed[i]=false;
    }
    for(int i = 0;i<num;i++){
        for(int j = 0;j<num;j++){
            cin>>edge[i][j];
        }
    }
    for(int i = 0;i<num;i++){
        if(!traversed[i]){
            vector<int> tmp;
            tmp = traverse(tmp);
            sort(tmp.begin(),tmp.end());
            for(size_t j = 0;j<tmp.size();j++){
                cout<<tmp[j]<<" ";
            }
            cout<<endl;
        }
    }
    return 0;
}

