#include<iostream>
#include<queue>
#include<utility>
#include<iomanip>
#include<float.h>
using namespace std;

struct furniture{
	double value;
	double people;
	double point;
};

struct cmp{
	bool operator()(furniture a, furniture b){
		if(a.point!=b.point) return a.point<b.point;
		else return a.value<b.value;
	}
};

int main(){
	priority_queue<furniture,vector<furniture>,cmp> pq;
	int num_furniture,team;
	cin>>num_furniture>>team;
	for(int i = 0;i<num_furniture;i++){
		double value,people;
		cin>>value>>people;
		furniture tmp = {value,people,value/people};
		pq.push(tmp);
	}
	double min = DBL_MAX;
	while(team--){
		furniture tmp = pq.top();
		pq.pop();
		double tmp_value = tmp.value;
		double tmp_people = tmp.people+1;
		double tmp_point = tmp_value/tmp_people;
		if(tmp_point<min) min = tmp_point;
		if(team==0){
			cout<<setprecision(11)<<min;
			break;
		}
		pq.push({tmp_value,tmp_people,tmp_point});
	}
}
