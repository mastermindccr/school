#include<iostream>
#include<utility>
#include<queue>
#include<iomanip>
using namespace std;
typedef pair<double,double> pd;

vector<pd> furniture;
vector<double> team; // number of teammates on each furniture

double calc(double a, double b, double c){
	if(b == 0) return 0;
	return a*b/(b+c);
}

struct cmp{ // total_value*team/(team+original_people)
	bool operator()(int a,int b){
		double earn_a = calc(furniture[a].first,team[a]+1,furniture[a].second)-calc(furniture[a].first,team[a],furniture[a].second);
		double earn_b = calc(furniture[b].first,team[b]+1,furniture[b].second)-calc(furniture[b].first,team[b],furniture[b].second);
		return earn_a<earn_b;
	}
};

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N,K;
	cin>>N>>K;
	furniture.resize(N);
	team.resize(N,0);
	priority_queue<int,vector<int>,cmp> pq;
	for(int i = 0;i<N;i++){
		double a,b;
		cin>>a>>b;
		furniture[i] = make_pair(a,b);
		pq.push(i);
	}
	for(int i = 0;i<K;i++){
		int cur = pq.top();
		pq.pop();
		team[cur]++;
		pq.push(cur);
	}
	double total = 0;
	for(int i = 0;i<N;i++){
		if(team[i]==0) continue;
		total+=furniture[i].first*team[i]/(team[i]+furniture[i].second);
	}
	cout<<setprecision(8)<<total;
}
