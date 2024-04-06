#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;

int n,m,v1,v2;
bool isend;
vector<vector<int>> line;
vector<char> color;

void bipartite(int vertex,char clr){
	for(int i = 0;i<n;i++){
		if(line[vertex][i]==1){
			if(clr == 'b'){
				if(color[i]=='b'){
					cout<<"NO";
					isend = true;
					return;
				}
				else{
					color[i] = 'g';
				}
			}
			else{
				if(color[i]=='g'){
					cout<<"NO";
					isend = true;
					return;
				}
				else{
					color[i] = 'b';
				}
			}
			line[vertex][i] = 0;
			line[i][vertex] = 0;
			bipartite(i,color[i]);
			if(isend) return;
		}
	}
}
int main(){
	cin>>n>>m;
	line.resize(n,vector<int>(n,0));
	color.resize(n,'n');
	while(m--){
		cin>>v1>>v2;
		line[v1][v2]=1;
		line[v2][v1]=1;
	}
	for(int i = 0;i<n;i++){
		if(color[i]=='n'){
			color[i] = 'b';
			bipartite(i,color[i]);
			if(isend) break;
		}
	}
	if(!isend) cout<<"YES";
}
