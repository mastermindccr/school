#include<iostream>
#include<vector>

using namespace std;

int main(){
	int n,m,v1,v2;
	int longest = 0;
	cin>>n>>m;
	vector<vector<int>> con(n,vector<int>(n,0));
	vector<int> vertices(n);//unfinished vertices
	vector<int> indeg(n,0);
	vector<int> dis(n,0);
	for(int i = 0;i<n;i++){
		vertices[i] = i;
	}
	while(m--){
		cin>>v1>>v2;
		con[v1][v2] = 1;
		indeg[v2]++;
	}
	while(!vertices.empty()){
		bool DAG = false;
		for(int i = 0;i<vertices.size();i++){
			if(indeg[vertices[i]]==0){
				DAG = true;
				for(int j = 0;j<n;j++){
					if(con[vertices[i]][j]==1){
						dis[j] = max(dis[vertices[i]]+1,dis[j]);
						longest = max(longest,dis[j]);
						indeg[j]--;
						con[vertices[i]][j]=0;
					}
				}
				vertices.erase(vertices.begin()+i);
			}
		}
		if(!DAG){
			longest = -1;
			break;
		}
	}
	cout<<longest;
}
