#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;

int main() {
    int v_num,e_num;
    int node1,node2,weight;
    int sum = 0;
    cin>>v_num>>e_num;
    vector<pair<int,pair<int,int>>> edge(e_num);//weight,node1,node2
    vector<int> group(v_num);
    for(int i = 0;i<v_num;i++){
        group[i] = i;
    }
    for(int i = 0;i<e_num;i++){
        cin>>node1>>node2>>weight;
        edge[i] = make_pair(weight,make_pair(node1,node2));
    }
    //kruskal's algo
    sort(edge.begin(),edge.end());
    for(int i = 0;i<e_num;i++){
        if(group[edge[i].second.first] != group[edge[i].second.second]){
            sum+=edge[i].first;
            //union
            int tmp = group[edge[i].second.first];
            for(int j = 0;j<v_num;j++){
                if(group[j] == tmp) group[j] = group[edge[i].second.second];
            }
        }
    }
    cout<<sum;
    return 0;
}
