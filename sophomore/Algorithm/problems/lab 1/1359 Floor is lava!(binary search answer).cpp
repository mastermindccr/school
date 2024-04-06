#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
using namespace std;

struct furniture{
	double value;
	double people;
};

int main(){
	double n,k;
	cin>>n>>k;
	vector<furniture> vec(n);
	for(int i = 0;i<n;i++){
		cin>>vec[i].value>>vec[i].people;
	}
	// use binary search on answer
	double left = 0;
	double right = 1000000000;
	double ans = 0;
	double mid;
	while(right-left>0.000001){ // when the attempted value is close enough to the real answer
		ans = 0;
		mid = (left+right)/2;
		for(auto i : vec){
			ans+= max(i.value/mid-i.people,(double)0);
			// the attempted value is too small -> thus try it larger
			if(ans-k > 0.000001){
				left = mid;
				break;
			}
		}	
		if(k-ans > 0.000001) right = mid;// the attempted value is too large -> thus try it smaller
		else left = mid; // get answer as big as possible to reach the real answer
	}
	cout<<setprecision(8)<<mid;
}
