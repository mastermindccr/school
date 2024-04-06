#include<bits/stdc++.h>

using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int L, N;
	cin>>L>>N;
	
	set<int> jus;
	multiset<int> lens;
	jus.insert(0);
	jus.insert(L);
	lens.insert(L);
	
	while(N--){
		int ju;
		cin>>ju;
		jus.insert(ju);
		int n = *next(jus.find(ju));
		int p = *prev(jus.find(ju));
		
		lens.erase(lens.find(n-p));
		lens.insert(n-ju);
		lens.insert(ju-p);
		cout<<*prev(lens.end())/2<<'\n';
	}
}
