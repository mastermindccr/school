#include<iostream>
#include<queue>
#include<utility>
using namespace std;

int main(){
	int num_of_students;
	cin>>num_of_students;
	queue<pair<int,int>> que;
	while(num_of_students--){
		int id,potential;
		cin>>id>>potential;
		que.push(make_pair(id,potential));
	}
	while(!que.empty()){
		cout<<que.front().first<<'\n';
		if(que.front().second>1) que.push(make_pair(que.front().first,que.front().second-1));
		que.pop();
	}
}
