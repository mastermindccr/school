#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,price,Lt,Rt,Li,Ri;
	cin>>N>>price>>Lt>>Rt;
	vector<pair<int,int>> stalls; // {start,end}
	for(int i = 0;i<N;i++){
		cin>>Li>>Ri;
		if(Li<Rt) stalls.push_back({Li,Ri});
	}
	sort(stalls.begin(),stalls.end());
	
	int cur_val = Lt; // stalls that have been found til now from Lt
	long long stall_num = 0;
	auto last = stalls.begin(); // last position traversed by
	bool found = true;
	while(cur_val<Rt){
		auto it = upper_bound(last,stalls.end(),make_pair(cur_val,1000000000)); // pairs before it is considered into a possible new pair
		if(it==last){ // there is no new stall
			found = false;
			break;
		}
		for(auto i = last;i!=it;i++){
			if((*i).second>cur_val){
				cur_val = (*i).second;
			}
		}
		stall_num++;
		last = it;
	}
	if(!found) cout<<0;
	else cout<<(stall_num*price);
}
