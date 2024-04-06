#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;
typedef pair<long long,int> pli;
int N,M;

void dijkstra(int cur,vector<vector<pi>>& adj, long long* dist){
	bool traversed[N+1];
	memset(traversed,false,sizeof(traversed));		
	priority_queue<pli,vector<pli>,greater<pli>> que; // distance:node_number
	que.push(make_pair(0,cur));
	while(!que.empty()){		
		pair<long long,int> tmp = que.top(); // distance of cur node from beg : cur_node
		que.pop();
		if(!traversed[tmp.second]){
			traversed[tmp.second] = true;
			dist[tmp.second] = tmp.first;
			for(auto &i : adj[tmp.second]){
				if(!traversed[i.first]){
					que.push(make_pair(dist[tmp.second]+i.second,i.first));
				}
			}
		}
	}
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>N>>M;
	vector<vector<pi>> adj(N+1);
	vector<vector<pi>> rev_adj(N+1);
	vector<tuple<int,int,int>> edge; // start,end,weight
	long long min_cost[M]; // min_cost of passing each edge
	long long dist_beg[N+1],dist_fin[N+1];
	for(int i = 0;i<N+1;i++){
		dist_beg[i] = dist_fin[i] = (long long)1e18;
	}
	int tmp1,tmp2,weight;
	for(int i = 0;i<M;i++){
		cin>>tmp1>>tmp2>>weight;
		edge.push_back(make_tuple(tmp1,tmp2,weight));
		adj[tmp1].push_back(make_pair(tmp2,weight)); // connected node:weight
		rev_adj[tmp2].push_back(make_pair(tmp1,weight)); // connected node:weight
	}
	dijkstra(1,adj,dist_beg);
	dijkstra(N,rev_adj,dist_fin);
	long long min = (long long)1e18;
	for(int i = 0;i<M;i++){
		min_cost[i] = dist_beg[get<0>(edge[i])] + dist_fin[get<1>(edge[i])] + get<2>(edge[i]);
		if(min_cost[i]<min) min = min_cost[i];
	}
	for(int i = 0;i<M;i++){
		if(min_cost[i]-min <= get<2>(edge[i])) cout<<min_cost[i]-min<<'\n';
		else cout<<-1<<'\n';
	}
}
