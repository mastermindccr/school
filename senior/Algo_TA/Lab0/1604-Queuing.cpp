#include<bits/stdc++.h>

using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	
	int t;
	cin>>t;
	queue<pair<long long, int>> q; // queue to save <number of mask, number of person specific to this number>
	int new_people = 0; // record new queued people since last time of distributing mask
	long long max_mask = 0;
	while(t--){
		int op, num;
		cin>>op>>num;
		if(op==1){ // distribute mask
			max_mask += num;
			q.push(make_pair(num, new_people));
			new_people = 0;
		}
		else if(op==2){ // people leave
			while(!q.empty() && num>0){
				pair<long long, int> tmp = q.front();
				if(num>=tmp.second){
					num -= tmp.second;
					q.pop();
					max_mask -= tmp.first;
				}
				else{
					q.front() = make_pair(tmp.first, tmp.second-num);
					num = 0;
				}
			}
			while(!q.empty() && q.front().second==0){ // remove potential mask with no new people in the front of queue
				pair<long long, int> tmp = q.front();
				q.pop();
				max_mask -= tmp.first;
			}
			if(num>0){ // people with masks all leave, subtract from new people
				new_people -= num;
			}
		}
		else{ // people queue
			new_people += num;
		}
		cout<<max_mask<<'\n';
	}
}
