#include<bits/stdc++.h>

using namespace std;

typedef pair<int, long long> pil;
typedef struct {
	int s;
	int e;
	long long x;
} auction;

bool cmp(auction &a, auction &b){
	return a.e<b.e;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int n;
	cin>>n;
	vector<auction> vec(n);
	vector<pil> dp; // {time, max value get before a certain end time}
	dp.push_back({0, 0});
	for(auto &i: vec){
		cin>>i.s>>i.e>>i.x;
	}
	sort(vec.begin(), vec.end(), cmp);
	for(auto &i: vec){
		long long weight = (*prev(lower_bound(dp.begin(), dp.end(), make_pair(i.s, (long long)0)))).second;
		if((*dp.rbegin()).first==i.e) (*dp.rbegin()).second = max(weight+i.x, (*dp.rbegin()).second);
		else dp.push_back(make_pair(i.e, max(weight+i.x, (*dp.rbegin()).second)));
	}
	cout<<(*dp.rbegin()).second;
}
