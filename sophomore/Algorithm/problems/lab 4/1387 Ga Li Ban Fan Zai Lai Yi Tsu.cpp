#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
using namespace std;
unordered_map<long long,int> mp;

int dp(long long money, vector<long long> &vec, int index){
	if(money==0) return 0;
	if(mp[money]!=0) return mp[money];
	if(money<0) return 1000000000;
	if(index>=vec.size()) return 1000000000;
	return mp[money] = min(dp(money-vec[index],vec,0)+1,dp(money,vec,index+1));
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	vector<long long> denominations(N);
	for(int i = 0;i<N;i++){
		cin>>denominations[i];	
	}
	vector<long long> sorted(denominations.begin(),denominations.end());
	sort(sorted.begin(),sorted.end(),greater<long long>());
	int cus;
	cin>>cus;
	while(cus--){
		long long pay;
		cin>>pay;
		long long sum = 0;
		long long tmp;
		for(int i = 0;i<N;i++){
			cin>>tmp;
			sum+=denominations[i]*tmp;
		}
		cout<<dp(sum-pay,sorted,0)<<'\n';
	}	
}
