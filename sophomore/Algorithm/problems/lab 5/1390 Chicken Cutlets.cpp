#include<bits/stdc++.h>
using namespace std;
int V,E;

vector<vector<int>> adj;
vector<vector<int>> reversed_adj; // for kosaraju
vector<int> scc_head; // head of each scc's nodes
vector<unordered_set<int>> scc_adj; // adjacency list of sccs
vector<int> scc_size; // size of each sccs
vector<int> dist; // the most people it can tell starting from this scc

void dfs(int tmp, stack<int>& fin, bool* traversed){
	for(auto &i : adj[tmp]){
		if(!traversed[i]){
			traversed[i] = true;
			dfs(i,fin,traversed);
		}
	}
	fin.push(tmp);
}

void kosaraju(){
	stack<int> fin; // record finish time
	bool traversed[V];
	for(int i = 0;i<V;i++){
		traversed[i] = false;
	}
	for(int i = 0;i<V;i++){
		if(!traversed[i]){
			traversed[i] = true;
			dfs(i,fin,traversed);
		}
	}
	scc_size.resize(V);
	for(int i = 0;i<V;i++){ // initialize sccs's size with 0
		scc_size[i] = 0;
		traversed[i] = false;
	}
	while(!fin.empty()){
		int tmp = fin.top();
		fin.pop();
		if(!traversed[tmp]){
			scc_size[tmp] = 1;
			traversed[tmp] = true;
			stack<int> stk; // for dfs in reversed graph
			stk.push(tmp);
			while(!stk.empty()){
				int stk_top = stk.top();
				stk.pop();
				for(auto &i : reversed_adj[stk_top]){
					if(!traversed[i]){
						traversed[i] = true;
						stk.push(i);
						scc_head[i] = tmp;
						scc_size[tmp]++;
					}
				}
			}
		}
	}
	for(int i = 0;i<V;i++){
		for(auto &p : adj[i]){
			if(scc_head[i]!=scc_head[p]) // they are in different sccs
				scc_adj[scc_head[i]].insert(scc_head[p]);
		}		
	}
}

int find_dist(int cur){
	if(dist[cur]!=0) return dist[cur];
	if(scc_adj[cur].size()==0) return dist[cur] = scc_size[cur];
	int max = 0;
	for(auto &i : scc_adj[cur]){
		if(find_dist(i)>max) max = dist[i];
	}
	return dist[cur] = max + scc_size[cur];
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>V>>E;
	adj.resize(V);
	reversed_adj.resize(V);
	scc_adj.resize(V);
	scc_head.resize(V);
	dist.resize(V,0);
	for(int i = 0;i<V;i++){
		scc_head[i] = i;
	}
	int tmp1,tmp2;
	while(E--){
		cin>>tmp1>>tmp2;
		adj[tmp1].push_back(tmp2);
		reversed_adj[tmp2].push_back(tmp1);
	}
	kosaraju();
	int ans = 0;
	for(int i = 0;i<V;i++){ // traverse all sccs
		if(scc_size[i]!=0){ // it's a scc
			if(dist[i] == 0){ // haven't found its distance yet
				if(find_dist(i)>ans) ans = dist[i];
			}					
		}
	}
	cout<<ans;
}
