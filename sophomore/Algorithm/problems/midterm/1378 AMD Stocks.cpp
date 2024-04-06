#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;

bool cmp(pi a, pi b){
	return a.second<b.second;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,K;
	cin>>N>>K;
	vector<pi> vec;
	for(int i = 0;i<N;i++){
		int start,end;
		cin>>start>>end;
		vec.emplace_back(make_pair(start,end));
	}
	sort(vec.begin(),vec.end(),cmp);
	int attend = 0;
	multiset<int> auctions;
	//conclusion: pushfront new end and delete the greatest number smaller than new first
	//use multiset to implement!(remove from set is log(n) time)
	for(auto &i : vec){
		if(auctions.size()==K && i.first<=*auctions.begin()) continue;
		if(auctions.size()!=0 && i.first>=*auctions.begin()){
			auctions.erase(prev(upper_bound(auctions.begin(),auctions.end(),i.first-1)));
		}		
		auctions.insert(i.second);
		attend++;
	}
	cout<<attend;
}
