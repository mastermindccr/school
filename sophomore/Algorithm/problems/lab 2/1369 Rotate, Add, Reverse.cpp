#include<iostream>
#include<algorithm>
using namespace std;
long long pow(long long a, long long b){
	if(b==0) return 1;
	return pow(a,b-1) * a;
}

int main(){
	int N;
	cin>>N;
	vector<long long> vec;
	for(int i = 0;i<=N;i++){
		if(i==0){
			vec = {1};
			continue;
		}
		vector<long long> A = vec;
		reverse(A.begin(),A.end());
		vector<long long> B = vec;
		for(int j = 0;j<vec.size();j++){
			B[j]+=pow(2,i-1);
		}
		B.insert(B.end(),A.begin(),A.end());
		vec = B;
	}
	for(auto &i : vec){
		cout<<i<<' ';
	}
}
