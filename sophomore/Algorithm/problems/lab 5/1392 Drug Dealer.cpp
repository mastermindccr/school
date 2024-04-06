#include<bits/stdc++.h>
using namespace std;
int T,M,N,S,X,Y;

struct Step{
	char person;
	int number; // node number
};

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>T;
	while(T--){
		cin>>N>>M>>S;
		vector<vector<int>> edge(N+1);
		bool hideout[N+1];
		bool traversed[N+1];
		memset(hideout,false,sizeof(hideout));
		memset(traversed,false,sizeof(traversed));
		int tmp1,tmp2;
		while(M--){
			cin>>tmp1>>tmp2;
			edge[tmp1].push_back(tmp2);
			edge[tmp2].push_back(tmp1);
		}
		//use a queue to record every one's position to the hideout
		queue<Step> que;
		traversed[S] = true;
		cin>>X;
		for(int i = 0;i<X;i++){
			cin>>tmp1;
			que.push({'p',tmp1});
			traversed[tmp1] = true;
		}
		que.push({'y',S});
		cin>>Y;
		for(int i = 0;i<Y;i++){
			cin>>tmp1;
			hideout[tmp1] = true;
		}
		int num_found = 0; // number of hideouts that has been found
		while(!que.empty()){
			
			Step tmp = que.front();
			que.pop();
			if(tmp.person == 'p' && hideout[tmp.number]){
				hideout[tmp.number] = false;
				num_found++;
			}
			else if(tmp.person == 'y' && hideout[tmp.number]){
				cout<<"Yes"<<'\n';
				break;
			}
			for(auto &i : edge[tmp.number]){
				if(!traversed[i]){
					traversed[i] = true;
					que.push({tmp.person,i});
				}
			}
			if(num_found==Y){
				cout<<"No"<<'\n';
				break;
			}
		}
	}
}
