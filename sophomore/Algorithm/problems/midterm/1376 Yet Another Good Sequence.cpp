#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

long long same(vector<long long> vec){
	if(vec.size()<=1) return 0;
	int max = vec[0];
	for(auto &i : vec){
		if(i>max) max = i;
	}
	long long ret = 0;
	for(auto &i : vec){
		ret+=(max-i);
	}
	return ret;
}

long long good(vector<long long>& vec){
	if(vec.size()<=3) return 0;
	int size = (vec.size()+1)/2;
	vector<long long> first(vec.begin(),vec.begin()+size);
	vector<long long> second(vec.begin()+size,vec.end());
	return std::min(good(first)+same(second),good(second)+same(first));
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	vector<long long> vec(num);
	for(int i = 0;i<num;i++){
		cin>>vec[i];
	}
	cout<<good(vec);
}
