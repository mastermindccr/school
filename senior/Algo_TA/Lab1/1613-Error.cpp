#include<bits/stdc++.h>

using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int n, q;
	cin>>n>>q;
	vector<int> order(n+1);
	vector<pair<int, int>> interval(q+1);
	
	// record where should the number come to
	for(int i = 1;i<=n;i++){
		int a;
		cin>>a;
		order[a] = i;
	}
	for(int i = 1;i<=q;i++){
		int a, b;
		cin>>a>>b;
		interval[i] = make_pair(a, b);
	}
	
	// scan for unsorted interval
	int lo = 1;
	int hi = n;
	
	for(int i = 1;i<=n;i++){
		if(order[i]!=i){
			break;
		}
		lo++;
	}
	
	for(int i = n;i>=1;i--){
		if(order[i]!=i){
			break;
		}
		hi--;
	}
	
	for(int i = 1;i<=q;i++){
		// scan order according to the given interval and unsorted interval
		int lo_bound = max(lo, interval[i].first);
		int up_bound = min(hi, interval[i].second);
		
		// initial array for counting sort
		vector<int> cnt(n+1, 0);
		for(int i = lo_bound;i<=up_bound;i++){
			cnt[order[i]]++;
		}
		
		int ptr = lo_bound;
		// write the counted number back to the order array
		for(int i = 1;i<=n && ptr<=up_bound;i++){
			if(cnt[i]){
				order[ptr] = i;
				ptr++;
			}
		}
		
		int tmp_lo = lo, tmp_hi = hi;
		
		// scan the interval to update the unsorted interval
		for(int i = tmp_lo;i<=tmp_hi;i++){
			if(order[i]!=i){
				break;
			}
			lo++;
		}
		
		for(int i = tmp_hi;i>=tmp_lo;i--){
			if(order[i]!=i){
				break;
			}
			hi--;
		}
		if(lo>=hi){
			cout<<i<<'\n';
			exit(0);
		}
	}
	cout<<-1<<'\n';
}
