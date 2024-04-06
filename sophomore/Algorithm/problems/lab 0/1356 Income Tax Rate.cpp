#include<iostream>
#include<vector>
#include<utility>
#include<iomanip>

using namespace std;

int main(){
	int range;
	cin>>range;
	vector<pair<double,double>> ranges;
	for(int i = 0;i<range;i++){
		double fir,sec;
		cin>>fir>>sec;
		ranges.push_back(make_pair(fir-1,sec));
	}
	int num_query;
	cin>>num_query;
	while(num_query--){
		double income;
		cin>>income;
		double pay = 0;
		for(int i = 1;i<ranges.size();i++){
				if(income>ranges[i].first){
					pay+=(ranges[i].first-ranges[i-1].first)*ranges[i-1].second;
					if(i==ranges.size()-1){
						pay+=(income-ranges[i].first)*ranges[i].second;
					}
				}
				else{
					if(income>ranges[i-1].first){
						pay+=(income-ranges[i-1].first)*ranges[i-1].second;
					}
					break;
				}
		}
		cout<<setprecision(15)<<pay<<'\n'; //cout will only print 6 numbers after decimal by default 
	}
}
