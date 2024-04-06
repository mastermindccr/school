#include<bits/stdc++.h>
using namespace std;
int T,N,M,K,F,S;

int par(int node, int* arr){
	if(node == arr[node]) return node;
	return arr[node] = par(arr[node],arr);
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>T;
	while(T--){
		cin>>N>>M>>K>>F>>S;
		int disjoint_all[N];
		int disjoint_fast[N];
		int disjoint_slow[N];
		for(int i = 0;i<N;i++){
			disjoint_all[i] = i;
			disjoint_fast[i] = i;
			disjoint_slow[i] = i;
		}
		int fast_upper = 0; // how many fast routes can contribute at most
		int slow_upper = 0; // how many slow routes can contribute at most
		int tmp1,tmp2;
		char type;
		while(M--){
			cin>>tmp1>>tmp2>>type;
			if(par(tmp1,disjoint_all)!=par(tmp2,disjoint_all)){ // build mst using every edge
				disjoint_all[par(tmp1,disjoint_all)] = par(tmp2,disjoint_all);
			}
			if(type=='F'){
				if(par(tmp1,disjoint_fast)!=par(tmp2,disjoint_fast)){
					disjoint_fast[par(tmp1,disjoint_fast)] = par(tmp2,disjoint_fast);
					fast_upper++;
				}
			}
			else{
				if(par(tmp1,disjoint_slow)!=par(tmp2,disjoint_slow)){
					disjoint_slow[par(tmp1,disjoint_slow)] = par(tmp2,disjoint_slow);
					slow_upper++;
				}
			}
		}
		int slow_lower = N-1-fast_upper; // how many slow routes needed at least
		int fast_lower = N-1-slow_upper; // how many fast routes needed at least
		
		bool ans = true;
		for(int i = 0;i<N-1;i++){
			if(par(i,disjoint_all)!=par(i+1,disjoint_all)){
				ans = false;
				break;
			}
		}
		if(!ans){ //  cannot form mst if all the edges are put in
			cout<<"No"<<'\n';
			continue;
		}
		
		if(F==S){ // if cost of two roads are the same
			if(F*(N-1)==K) // then if there's N-1 route can form mst, it's valid, otherwise not
				cout<<"Yes"<<'\n';
			else
				cout<<"No"<<'\n';
			continue;
		}
		
		// needed number of each road to meet the cost
		int fast = (K-S*N+S)/(F-S);
		int slow = ((long long)F*N-F-K)/(F-S); // must assign long long otherwise will overflow!!
		// positive, road num = N-1, upper_bounded and lower_bounded
		if(fast>=0 && slow>=0 && fast+slow==N-1 && fast_lower<=fast && fast<=fast_upper && slow_lower<=slow && slow<=slow_upper) 
			cout<<"Yes"<<'\n';
		else
			cout<<"No"<<'\n';
	}
}
