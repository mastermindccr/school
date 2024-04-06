#include<bits/stdc++.h>
#define max INT_MAX
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	
	int n, m, num, avail = 0;
	cin>>n>>m;
	set<int> init;
	set<pair<int,int>> s; // record the interval that is not in the original set
	
	for(int i = 0;i<n;i++){
		cin>>num;
		init.insert(num);
	}
	
	for(auto &i: init){
		if(avail<=i-1){
			s.insert(make_pair(avail, i-1));
		}
		avail = i+1;
	}
	s.insert(make_pair(avail, max));
	
	while(m--){
		cin>>num;
		if(num==1){ // remove the smallest
			auto small = *s.begin();
			auto second_small = *next(s.begin());
			
			if(small.first!=0){ // 0 is in the original set, remove 0
				if(small.first==1){
					s.erase(s.begin());
					s.insert(make_pair(0, small.second));
				}
				else{
					s.insert(make_pair(0, 0));
				}
				cout<<0;
			}
			else{
				// if the second smallest overlap with the smallest after removal, then merge them
				if(second_small.first == small.second+2){
					s.erase(s.begin());
					s.erase(s.begin());
					s.insert(make_pair(small.first, second_small.second));
				}
				else{
					s.erase(s.begin());
					s.insert(make_pair(small.first, small.second+1));
				}
				cout<<small.second+1;
			}
			
		}
		else if(num==2){ // remove the largest
			auto large = *prev(s.end());
			auto second_large = *prev(prev(s.end()));
			
			// if the second largest overlap with the largest after removal, then merge them
			if(second_large.second+2 == large.first){
				s.erase(prev(s.end()));
				s.erase(prev(s.end()));
				s.insert(make_pair(second_large.first, large.second));
			}
			else{
				s.erase(prev(s.end()));
				s.insert(make_pair(large.first-1, large.second));
			}
			cout<<large.first-1;
		}
		else{ // insert the smallest number
			auto small = *s.begin();
			cout<<small.first;
			
			if(small.first==small.second){ // only one number in the interval
				s.erase(s.begin());
			}
			else{
				s.erase(s.begin());
				s.insert(make_pair(small.first+1, small.second));
			}
		}
		cout<<'\n';
	}
}
