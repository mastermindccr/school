#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
class edge{
public:
    edge(int a,int b,int c):weight(a),start(b),end(c){};
    int weight;
    int start;
    int end;
};
using namespace std;

int main() {
    int v_num,e_num;
    int start,end;
    int node1,node2,weight;
    bool neg = false;
    cin>>v_num>>e_num>>start>>end;
    vector<edge*> edges;
    vector<int> dis(v_num,9999999);
    for(int i = 0;i<e_num;i++){
        cin>>node1>>node2>>weight;
        edges.push_back(new edge(weight,node1,node2));
    }
    //Bellman-ford
    dis[start] = 0;
    for(int i = 0;i<v_num-1;i++){
        for(int i = 0;i<e_num;i++){
            if(edges[i]->weight + dis[edges[i]->start] < dis[edges[i]->end]){
                dis[edges[i]->end] = edges[i]->weight + dis[edges[i]->start];
            }
        }
    }
    //detect neg-cycle
    for(int i = 0;i<e_num;i++){
        if(edges[i]->weight + dis[edges[i]->start] < dis[edges[i]->end]){
            neg = true;
            break;
        }
    }
    if(neg) cout<<"Negative loop detected!";
    else cout<<dis[end];
    return 0;
}
