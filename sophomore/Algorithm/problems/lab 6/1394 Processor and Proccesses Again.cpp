#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;
int T,M;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>T;
	// build graph to find max flow
	
	vector<vector<pi>> adj(T+2); // 0:src, T+1:sink
	int tmp1,tmp2,cost;
	for(int i = 0;i<T;i++){
		cin>>tmp1;
		adj[0].push_back({i+1,tmp1});
	}
	for(int i = 0;i<T;i++){
		cin>>tmp1;
		adj[i+1].push_back({T+1,tmp1});
	}
	cin>>M;
	while(M--){
		cin>>tmp1>>tmp2>>cost;
		bool found = false;
		for(auto &i : adj[tmp1]){
			if(i.first == tmp2){
				found = true;
				i.second += cost;
				break;
			}
		}
		if(found){
			for(auto &i : adj[tmp2]){
				if(i.first == tmp1){
					i.second += cost;
					break;
				}
			}
		}
		else{
			adj[tmp1].push_back({tmp2,cost});
			adj[tmp2].push_back({tmp1,cost});
		}
	}
	// finish building graph
	// below is edmonds-karp
	long long max_flow = 0;
	while(true){
		vector<int> pred(T+2);
		fill(pred.begin(),pred.end(),-1);
		pred[0] = 0;
		queue<pi> que; // node:min flow on this path
		que.push({0,INT_MAX});
		while(!que.empty()){
			pi tmp = que.front();			
			if(tmp.first == T+1){ // it is sink node
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
	cout<<max_flow<<'\n';
}
