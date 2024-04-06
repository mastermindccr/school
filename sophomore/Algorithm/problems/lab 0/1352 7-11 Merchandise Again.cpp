#include<iostream>
#include<unordered_map>
#include<queue>
using namespace std;

int main(){
	cin.tie(0);
  	cin.sync_with_stdio(0);
	int N;
	int type,query_number,date;
	cin>>N;
	unordered_map<int,priority_queue<int,vector<int>,greater<int>>> Items;
	for(int t = 0;t<N;t++){
		cin>>type;
		if(type==1){
			cin>>query_number>>date;
			auto it = Items.find(query_number);
			if(it!=Items.end()){
				(*it).second.push(date);
			}
			else{
				priority_queue<int,vector<int>,greater<int>> temp;
				temp.push(date);
				Items.insert({query_number,temp});
			}
		}
		else if(type==2){
			cin>>query_number;
			auto it = Items.find(query_number);
			if(it!=Items.end() && !(*it).second.empty()){
				cout<<(*it).second.top()<<'\n';
				(*it).second.pop();
			}
			else{
				cout<<-1<<'\n';
			}
		}
	}
}
