#include<bits/stdc++.h>
using namespace std;
vector<int> vec;
vector<long long> sum;
vector<int> num;
int n,m;

int par(int a){
	if(vec[a] == a) return vec[a];
	return vec[a] = par(vec[a]);
}

void disjoint(int a, int b){
	if(par(a)!=par(b)){
		num[par(b)-n] += num[par(a)-n];
		num[par(a)-n] = 0;
		sum[par(b)-n] += sum[par(a)-n];
		sum[par(a)-n] = 0;
		vec[par(a)] = par(b);
	}
}

void take(int a, int b){
	if(par(a)!=par(b)){
		num[par(b)-n] ++;
		num[par(a)-n] --;
		sum[par(b)-n] += (long long)a;
		sum[par(a)-n] -= (long long)a;
		vec[a] = par(b);
	} 
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	while(cin>>n>>m){
		vec.resize(2*n+1);
		sum.resize(n+1);
		num.resize(n+1);
		for(int i = 1;i<=n;i++){
			vec[i] = i+n;
			vec[i+n] = i+n;
			sum[i] = (long long)i;
			num[i] = 1;
		}
		int cmd,tmp1,tmp2;
		while(m--){
			cin>>cmd>>tmp1;
			if(cmd==1){
				cin>>tmp2;
				disjoint(tmp1,tmp2);
			}
			else if(cmd==2){
				cin>>tmp2;
				take(tmp1,tmp2);
			}
			else{
				cout<<num[par(tmp1)-n]<<' '<<sum[par(tmp1)-n]<<'\n';
			}
		}
	}	
}
