#include<bits/stdc++.h>
long long dp[10000001]; // the number of one concavity including blank
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	dp[0] = 1;
	dp[1] = 1;
	dp[2] = 3;
	for(int i = 3;i<1e7+1;i++){
		// i-3: 0 1 0  i-2: 1 0     0 0   i-1: 0
		//      1 1 1       1 1  &  1 0        0
		//      1 1 0       0 0     1 1        0
		dp[i] = (dp[i-3]+2*dp[i-2]+dp[i-1])%(int)(1e9+7);
	}
	int N;
	cin>>N;
	int tmp;
	while(N--){
		cin>>tmp;
		cout<<(4*dp[tmp]-3)%(int)(1e9+7)<<'\n'; // four concavity thus minus 3 blanks
	}
}
