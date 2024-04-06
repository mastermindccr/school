#include<iostream>
#include<vector>
#include<queue>
#include<utility>
using namespace std;

typedef pair<int,int> pi;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int process_num,core_num,task_num;
	cin>>process_num>>core_num>>task_num;
	int switches = 0;
	int is_doing[process_num+1]; // -1 means not doing, other means its working index
	for(int i = 0;i<process_num+1;i++){
		is_doing[i] = -1;
	}
	vector<priority_queue<int,vector<int>,greater<int>>> pos(process_num+1); // positions each process appears
	vector<int> steps;
	for(int i = 0;i<task_num;i++){
		int process;
		cin>>process;
		pos[process].push(i);
		steps.push_back(process);
	}
	
	priority_queue<pi> replace; // to record each next position of each on-doing processor
	int fill = 0;
	for(int i = 0;i<task_num;i++){
		int cur = steps[i]; // current process
		pos[cur].pop();
		if(is_doing[cur]!=-1){ // the current process in doing in the core now
			if(pos[cur].empty()) // this is the last time this process appear
				replace.push(make_pair(task_num,cur));
			else // this is not the last time this process appear
				replace.push(make_pair(pos[cur].top(),cur));
			continue; 	
		} 
		if(fill<core_num){ // there is still an empty slot in the core
			switches++;
			if(pos[cur].empty()) // this is the last time this process appear
				replace.push(make_pair(task_num,cur));
			else // this is not the last time this process appear
				replace.push(make_pair(pos[cur].top(),cur));
			is_doing[cur] = fill;
			fill++;
			continue;
		}
		int remove = replace.top().second; // the index of core to be replaced
		replace.pop();
		is_doing[cur] = is_doing[remove]; // current process succeeds the replaced process's core
		is_doing[remove] = -1; // the replaced process is not doing now

		if(pos[cur].empty()) // this is the last time this process appear
				replace.push(make_pair(task_num,cur));
			else // this is not the last time this process appear
				replace.push(make_pair(pos[cur].top(),cur));
		switches++;
	}
	cout<<switches;
}
