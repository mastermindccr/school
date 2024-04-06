#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;
int head[200005]; // use disjoint set to record each person's root friend

struct Query{
	char cmd;
	int v1;
	int v2;
};

int par(int node){
	if(node==head[node]) return node;
	return head[node] = par(head[node]);
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,M,Q;
	cin>>N>>M>>Q;
	int tmp1,tmp2;
	set<pi> edge;
	for(int i = 0;i<N;i++){
		head[i] = i;
	}
	for(int i = 0;i<M;i++){
		cin>>tmp1>>tmp2;
		if(tmp1>tmp2) swap(tmp1,tmp2);
		edge.insert(make_pair(tmp1,tmp2));
	}
	// delete all queried edges to get the min size of graph, then connect it back in reversed order
	char cmd;
	stack<Query> stk; // save the queries
	for(int i = 0;i<Q;i++){
		cin>>cmd>>tmp1>>tmp2;
		if(cmd=='U'){
			if(tmp1>tmp2) swap(tmp1,tmp2);
			edge.erase(make_pair(tmp1,tmp2));
		}
		stk.push({cmd,tmp1,tmp2});
	}
	//merge set to build the min size of graph
	for(auto &p: edge){
		head[par(p.second)] = par(p.first);
	}
	
	stack<bool> ans;
	// answer queries in reversed order
	while(!stk.empty()){
		Query tmp = stk.top();
		stk.pop();
		if(tmp.cmd=='A'){
			ans.push(par(tmp.v1)==par(tmp.v2));
		}
		else{
			head[par(tmp.v2)] = par(tmp.v1); // renew the root node to merge into a larger set
		}
	}
	while(!ans.empty()){
		if(ans.top()) cout<<"YES"<<'\n';
		else cout<<"NO"<<'\n';
		ans.pop();
	}
}
