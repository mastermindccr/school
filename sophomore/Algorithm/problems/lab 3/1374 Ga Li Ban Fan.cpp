#include<iostream>
#include<vector>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,roommates;
	cin>>N;
	int denominations[7] = {1000,500,100,50,10,5,1};
	long long owe[7]; // needed number of every kind of denominations
	long long collected[7]; // number of every kind of denominations collected	
	while(N--){
		bool ans = true;
		for(int i = 0;i<7;i++){
			owe[i] = 0;
			collected[i] = 0;
		}
		
		cin>>roommates;
		for(int i = 0;i<roommates;i++){
			long long sum,num;
			cin>>sum;
			for(int j = 0;j<7;j++){
				cin>>num;
				sum -= num*denominations[j];
				collected[j] += num;
			}
			long long change = -sum;
			for(int j = 0;j<7;j++){
				owe[j] += change/denominations[j];
				change %= denominations[j];
			}
		}
		for(int i = 6;i>=0;i--){
			if(collected[i]<owe[i]){
				ans = false;
				break;
			}
			if(i != 0){
				collected[i-1] += (collected[i]-owe[i])/(denominations[i-1]/denominations[i]); // add the remaining to a larger denomination
			}
		}
		if(ans) cout<<"Yes";
		else cout<<"No";
		cout<<'\n';
	}
}
