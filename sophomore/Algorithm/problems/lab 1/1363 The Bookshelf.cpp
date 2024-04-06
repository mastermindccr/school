#include<iostream>
#include<unordered_map>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int n,m;
	cin>>n>>m;
	unordered_map<int,int> mp;
	for(int i = 0;i<n;i++){
		int height;
		cin>>height;
		mp[height] = i;
	}
	while(m--){
		int query;
		cin>>query;
		if(mp.find(query)!=mp.end()) cout<<mp[query];
		else cout<<-1;
		cout<<'\n';
	}
}
