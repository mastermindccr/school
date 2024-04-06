#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;
int F,N,M,C,T,K;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>F>>N>>M;
	// build graph to find max flow
	// src node : 0, farm node : 1~F
	// factory node : F+1~F+N, tmp node : F+N+1~F+N+N(produce num of each factory)
	// store node : F+N+N+1~F+N+N+M, sink node : F+N+N+M+1
	vector<vector<pi>> adj(F+N+N+M+2);
	int tmp,tmp2,weight;
	for(int i = 1;i<=F;i++){ // src <=> farm
		cin>>tmp;
		adj[0].push_back({i,tmp}); // forwarding edge
		adj[i].push_back({0,0}); // reversed edge
	}
	for(int i = F+1;i<=F+N;i++){ // farm <=> tmp
		cin>>tmp;
		adj[i].push_back({N+i,tmp}); // forwarding edge
		adj[N+i].push_back({i,0}); // reversed edge
	}
	for(int i = F+N+N+1;i<=F+N+N+M;i++){ // store <=> sink
		cin>>tmp;
		adj[i].push_back({F+N+N+M+1,tmp}); // forwarding edge
		adj[F+N+N+M+1].push_back({i,0}); // reversed edge
	}
	cin>>C;
	while(C--){ // farm <=> factory
		cin>>tmp>>tmp2>>weight;
		bool found = false;
		for(auto &i : adj[tmp]){
			if(i.first == F+tmp2){
				i.second += weight;
				found = true;
				break;
			}
		}
		if(!found){
			adj[tmp].push_back({F+tmp2,weight}); // forwarding edge
			adj[F+tmp2].push_back({tmp,0}); // reversed edge
		}
		
	}
	cin>>T;
	while(T--){ // tmp <=> store
		cin>>tmp>>tmp2>>weight;
		bool found = false;
		for(auto &i : adj[F+N+tmp]){
			if(i.first == F+N+N+tmp2){
				i.second += weight;
				found = true;
				break;
			}
		}
		if(!found){
			adj[F+N+tmp].push_back({F+N+N+tmp2,weight}); // forwarding edge
			adj[F+N+N+tmp2].push_back({F+N+tmp,0}); // reversed edge
		}		
	}
	cin>>K;
	// finish building graph
	// below is edmonds-karp
	long long max_flow = 0;
	while(true){
		vector<int> pred(F+N+N+M+2);
		fill(pred.begin(),pred.end(),-1);
		pred[0] = 0;
		queue<pi> que; // node:min flow on this path
		que.push({0,INT_MAX});
		while(!que.empty()){
			pi tmp = que.front();			
			if(tmp.first == F+N+N+M+1){ // it is sink node
				max_flow += tmp.second;
				while(tmp.first != 0){
					// forwarding path
					for(auto &i : adj[pred[tmp.first]]){
						if(i.first == tmp.first){
							i.second -= tmp.second;
							break;
						}
					}
					// reversed path
					for(auto &i : adj[tmp.first]){
						if(i.first == pred[tmp.first]){
							i.second += tmp.second;
							break;
						}
					}
					tmp.first = pred[tmp.first];
				}
				break;
			}
			que.pop();
			for(auto &i : adj[tmp.first]){
				if(pred[i.first] == -1 && i.second>0){ // not traversed and still has remaining capacity
					pred[i.first] = tmp.first;
					if(i.second<tmp.second) que.push({i.first,i.second});
					else que.push({i.first,tmp.second});
				}
			}
		}
		if(que.empty()){ // no augmenting path found!
			break;
		}
	}
	cout<<min(max_flow,(long long)K);
}
