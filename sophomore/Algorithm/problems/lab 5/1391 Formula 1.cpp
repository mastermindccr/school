#include<bits/stdc++.h>
using namespace std;

struct Edge{ // edge to find mst
	int v1;
	int v2;
	long long cost;
};

struct Edge2{
	int v; // connected vertex
	long long cost;
	long long pass; // how many times will the edge be passed throughout traversal
};

bool cmp(Edge a, Edge b){
	return a.cost<b.cost;
}

int par(vector<int> &vec, int node){
	if(vec[node]==node) return node;
	else return vec[node] = par(vec,vec[node]);
}

long long dfs(int node,vector<vector<Edge2>> &vec, vector<bool> &traversed){
	traversed[node] = true;
	int _return = 1;
	for(auto &i : vec[node]){
		if(!traversed[i.v]){
			i.pass = dfs(i.v,vec,traversed);
			_return += i.pass;
		}
	}
	return _return;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int V,E;
	cin>>V>>E;
	vector<Edge> edge; // all edges
	int v1,v2;
	long long cost;
	while(E--){
		cin>>v1>>v2>>cost;
		edge.push_back({v1,v2,cost});
	}
	sort(edge.begin(),edge.end(),cmp);
	// apply kruskal to find mst
	vector<vector<Edge2>> mst_edge(V+1); // edges for building mst
	vector<int> forest(V+1); // disjoint sets
	for(int i = 1;i<=V;i++){
		forest[i] = i;
	}
	for(auto &i : edge){
		if(par(forest,i.v1)!=par(forest,i.v2)){ // they are in different trees
			forest[par(forest,i.v1)] = par(forest,i.v2);
			mst_edge[i.v1].push_back({i.v2,i.cost,0});
			mst_edge[i.v2].push_back({i.v1,i.cost,0});
		}
	}
	for(int i = 1;i<V;i++){ // not all vertices can be traversed
		if(par(forest,i)!=par(forest,i+1)){
			cout<<-1;
			exit(0); // exit with number other than 0 will declare RE!!
		}
	}
	vector<bool> traversed(V+1); // whether vertex is traversed in calculating pass
	for(int i = 1;i<=V;i++){
		traversed[i] = false;
	}
	dfs(1,mst_edge,traversed); // dfs start from an arbitrary vertex and go through all edges to find pass
	
	long long ans = 0;
	long long ans2 = 0;
	for(auto &i : mst_edge){
		for(auto &j : i){
			ans = (ans + j.cost)%(long long)(1e9+7);
			ans2 = (ans2 + 2*j.cost*j.pass%(long long)(1e9+7)*(V-j.pass))%(long long)(1e9+7); // consecutive multiplication will cause overflow
		}                                                                // so do module after every mutiplication!
	}
	cout<<ans<<' '<<ans2;
}
