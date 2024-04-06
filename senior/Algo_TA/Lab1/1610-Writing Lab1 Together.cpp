#include<bits/stdc++.h>
using namespace std;

typedef struct {
	int start;
	int end;
	int index;
} student;

bool cmp(student a, student b){
	if(a.end!=b.end) return a.end>b.end;
	return a.start<b.start;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	vector<student> vec;
	vector<int> first(N, 0);
	vector<int> second(N, 0);
	for(int i = 0;i<N;i++){
		int s,e;
		cin>>s>>e;
		vec.push_back({s, e, i});
	}
	sort(vec.begin(), vec.end(), cmp);
	
	int min = vec[0].start;
	for(int i = 1;i<N;i++){
		if(vec[i].start==vec[i-1].start && vec[i].end==vec[i-1].end){ // have same interval with the previous one
			second[vec[i-1].index] = second[vec[i].index] = 1;
		}
		else if(vec[i].start>=min) second[vec[i].index] = 1;
		else min = vec[i].start;
	}
	
	min = vec[N-1].start;
	for(int i = N-2;i>=0;i--){
		if(vec[i].start==vec[i+1].start && vec[i].end==vec[i+1].end){ // have same interval with the previous one
			first[vec[i].index] = first[vec[i+1].index] = 1;
		}
		else if(vec[i].start<=min) first[vec[i].index] = 1;
		else min = vec[i].start;
	}
	for(int i = 0;i<N;i++){
		cout<<first[i]<<' ';
	}
	cout<<'\n';
	for(int i = 0;i<N;i++){
		cout<<second[i]<<' ';
	}
}
