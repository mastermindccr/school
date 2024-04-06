#include<bits/stdc++.h>
using namespace std;
int N,M;
typedef pair<int,long long> pi;
struct Edge{
	int u;
	int v;
	long long w;
};

bool cmp(Edge &a, Edge &b){
	return a.w<b.w;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>N>>M;
	vector<Edge> edge;
	set<long long> tmp; // edge that has no repetition
	int u,v;
	long long w;
	int min_neg = -1;
	for(int i = 0;i<M;i++){
		cin>>u>>v>>w;
		if(w<0) min_neg++;
		edge.push_back({u,v,w});
		tmp.insert(w);
	}
	sort(edge.begin(),edge.end(),cmp);
	vector<int> non_re(tmp.begin(),tmp.end());
	int left = -2;
	int right = (int)non_re.size()-1;
	int mid;
	while(left != right-1){
		mid = (left+right)/2;
		// bellman-ford to detect neg cycle
		vector<long long> dist(N);
		fill(dist.begin(),dist.end(),0);
		for(int i = 0;i<N-1;i++){
			for(int j = 0;j<edge.size();j++){
				if((mid==-1 || edge[j].w>non_re[mid]) && dist[edge[j].u]+edge[j].w<dist[edge[j].v])
					dist[edge[j].v] = dist[edge[j].u]+edge[j].w;
			}
		}
		bool neg = false;
		for(int i = 0;i<edge.size();i++){
			if((mid==-1 || edge[i].w>non_re[mid]) && dist[edge[i].u]+edge[i].w<dist[edge[i].v]){
				neg = true;
				break;
			}
		}
		if(neg) left = mid;
		else right = mid;
	}
	if(right==-1){
		cout<<"GO AHEAD";
	}
	else if(non_re[right]>=edge[min_neg].w){
		cout<<"NO WAY BACK";
	}
	else{
		cout<<non_re[right];
	}
}
