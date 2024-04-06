#include<bits/stdc++.h>
using namespace std;
typedef pair<long long, long long> pl;

pl calc(pl a, pl b, pl c){
	return make_pair(a.second*c.second-b.second+a.first+b.first+c.first,a.second+b.second+c.second);
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	pl dp[N][N+1]; // row: index, col: length for val:shrinked array
	for(int i = 0;i<N;i++){
		for(int j = 0;j<N+1;j++){
			dp[i][j] = make_pair(-1e18,0);
		}
	} 
	for(int i = 0;i<N;i++){ // length = 1
		cin>>dp[i][1].second;
		dp[i][1].first = 0;
	}
	for(int i = 0;i<N-2;i++){ // length = 3
		dp[i][3] = calc(dp[i][1],dp[i+1][1],dp[i+2][1]);
	}
	for(int i = 5;i<=N+1;i+=2){ // other lengths
		for(int j = 0;j<N+1-i;j++){
			// dp[j][i] = max((index1,length1)+(index2,length2)+(index3,length3))
			pl tmp;
			for(int a = i-2;a>=1;a--){
				for(int b = i-a-1;b>=1;b--){
					tmp = calc(dp[j][a],dp[j+a][b],dp[j+a+b][i-a-b]);
					if(tmp>dp[j][i]) dp[j][i] = tmp;
				}
			}
		}
	}
	cout<<dp[0][N].first;
}
