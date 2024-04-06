#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>
using namespace std;
typedef pair<int,int> pi;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,M,S;
	cin>>N>>M>>S;
	vector<int> games(N);
	for(int i = 0;i<N;i++){
		cin>>games[i];
	}
	sort(games.begin(),games.end());
	vector<pi> pandemics; // strength:games affected
	int beg,fin,strength;
	int remain = N; // games which are not affected by the pandemic
	for(int i = 0;i<M;i++){
		cin>>beg>>fin>>strength;
		int tmp = upper_bound(games.begin(),games.end(),fin) - upper_bound(games.begin(),games.end(),beg-1);
		pandemics.push_back(make_pair(strength,tmp));
		remain-=tmp;
	}
	int dp[S+1];
	for(int i = 0;i<=S;i++){
		dp[i] = 0;
	}
	for(int i = 0;i<M;i++){ // there are M pandemics so we renew M times
		for(int j = S;j-pandemics[i].first>=0;j--){ // currently take j scientists away
			dp[j] = max(dp[j],dp[j-pandemics[i].first]+pandemics[i].second); // dp[j]:no take, the other:take
		}
	}
	cout<<dp[S]+remain;
}
