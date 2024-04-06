#include<iostream>
using namespace std;

long long result;
long long record_pow; // record current power of a^(2^x)

void pow(long long N){
	if(N==0) return;
	if(N%2) result = (result*record_pow)%1000000007;
	record_pow = (record_pow*record_pow)%1000000007;
	pow(N/2);
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	long long a,b,N,days;
	cin>>a>>b>>N;
	while(N--){
		result = 1;
		record_pow = a;
		cin>>days;
		pow(days-1);
		// calculate the recursion formula by hand
		if(a!=1) result = (result*(1+b/(a-1))-(b/(a-1)))%1000000007;
		else result = (1+b*((days-1)%1000000007))%1000000007;
		if(result<0) result+=1000000007; // it is still possible a negative number after mod
		cout<<result<<'\n';
	}
}
