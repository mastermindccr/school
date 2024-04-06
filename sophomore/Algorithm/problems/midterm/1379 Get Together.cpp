#include<iostream>
#include<vector>
#include<utility>
#include<iomanip>
using namespace std;

typedef pair<int,int> pi; 

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	vector<pi> vec(num);
	for(int i = 0;i<num;i++){
		cin>>vec[i].first>>vec[i].second;
	}
	double left = 0;
	double right = 1000000000;
	double mid;
	while(right-left>0.000001){
		bool invalid = false;
		mid = (left+right)/2;
		double start = (double)vec[0].first-mid*vec[0].second;
		double end = (double)vec[0].first+mid*vec[0].second;
		for(int i = 1;i<num;i++){
			start = max(start,vec[i].first-mid*vec[i].second);
			end = min(end,vec[i].first+mid*vec[i].second);
			if(start>end){
				left = mid;
				invalid = true;
				break;
			}
		}
		if(invalid) continue;
		right = mid;
	}
	cout<<setprecision(8)<<mid;
}
