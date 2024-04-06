#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <queue>
using namespace std;

int main() {
    int e_num,v_num;
    int start,end;
    int node1,node2,weight;
    cin>>v_num>>e_num>>start>>end;
    vector<pair<int,int>> adj[v_num]; // first:weight second:node
    vector<pair<int,int>> dis; // first:distance second:node
    vector<bool> min(v_num,false);
    for(int i = 0;i<e_num;i++){
        cin>>node1>>node2>>weight;
        adj[node1].push_back(make_pair(weight,node2));
    }
    for(int i = 0;i<v_num;i++){
        dis.push_back(make_pair(9999999,i));
    }
    dis[start].first = 0;
    //Dijkstra
    priority_queue <pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;
    que.push(dis[start]);
    while(!que.empty()){
        int tmpdis = que.top().first;
        int tmpnode = que.top().second;
        if(end == tmpnode) break;
        min[tmpnode] = true;
        que.pop();
        for(size_t i = 0;i<adj[tmpnode].size();i++){
            if(min[adj[tmpnode][i].second] == false){
                if(adj[tmpnode][i].first + tmpdis < dis[adj[tmpnode][i].second].first){
                    dis[adj[tmpnode][i].second].first = adj[tmpnode][i].first + tmpdis;
                    que.push(dis[adj[tmpnode][i].second]);
                }
            }    
        }
    }
    cout<<dis[end].first;
    return 0;
}
