#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	vector<long long> first(num),second(num);
	for(int i = 0;i<num;i++){
		cin>>first[i];
	}
	for(int i = 0;i<num;i++){
		cin>>second[i];
	}
	sort(first.begin(),first.end());
	sort(second.begin(),second.end());
	long long product = 0;
	for(int i = 0;i<num;i++){
		product += first[i]*second[num-i-1];
	}
	cout<<product;
}
