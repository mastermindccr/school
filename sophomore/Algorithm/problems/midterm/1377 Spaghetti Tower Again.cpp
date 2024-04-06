#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	vector<int> vec(num);
	for(int i = 0;i<num;i++){
		cin>>vec[i];
	}
	sort(vec.begin(),vec.end());
	int query;
	cin>>query;
	int down,up;
	while(query--){
		cin>>down>>up;
		cout<<upper_bound(vec.begin(),vec.end(),up)-lower_bound(vec.begin(),vec.end(),down)<<'\n';
	}
}
