#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <queue>
using namespace std;


int main() {
    int v_num,e_num;
    int node1,node2,weight;
    cin>>v_num>>e_num;
    vector<pair<int,int>> adj[v_num];
    for(int i = 0;i<e_num;i++){
        cin>>node1>>node2>>weight;
        adj[node1].push_back(make_pair(node2,weight));
        adj[node2].push_back(make_pair(node1,weight));
    }
    //prim's algo
    vector<int> pre(v_num,-1);
    vector<int> key(v_num,9999999);
    vector<bool> visit(v_num,false);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;
    key[0] = 0;
    que.push(make_pair(key[0],0));
    while(!que.empty()){
        int num = que.top().second;
        que.pop();
        if(visit[num]==true) continue;
        visit[num] = 1;
        for(size_t i = 0;i<adj[num].size();i++){
            if(visit[adj[num][i].first]==false){
               if(adj[num][i].second<key[adj[num][i].first]){
                   key[adj[num][i].first] = adj[num][i].second;
                   pre[adj[num][i].first] = num;
                   que.push(make_pair(key[adj[num][i].first],adj[num][i].first));
                }
            }
        }
    }
    //sum
    int sum = 0;
    for(int i = 0;i<v_num;i++){
        sum+=key[i];
    }
    cout<<sum;
    return 0;
}

