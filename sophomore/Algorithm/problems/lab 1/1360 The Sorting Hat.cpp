#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
string G,H,R,S;

struct student{
	string name;
	int G;
	int H;
	int R;
	int S;
};

bool cmp_G(student a, student b){
	for(int i = 0;i<4;i++){
		if(G[i]=='G'){
			if(a.G!=b.G) return a.G>b.G;
			else continue;
		} 
		if(G[i]=='H'){
			if(a.H!=b.H) return a.H>b.H;
			else continue;
		} 
		if(G[i]=='R'){
			if(a.R!=b.R) return a.R>b.R;
			else continue;
		} 
		if(G[i]=='S'){
			if(a.S!=b.S) return a.S>b.S;
			else continue;
		} 
	}
	return a.name<b.name;
}

bool cmp_H(student a, student b){
	for(int i = 0;i<4;i++){
		if(H[i]=='G'){
			if(a.G!=b.G) return a.G>b.G;
			else continue;
		} 
		if(H[i]=='H'){
			if(a.H!=b.H) return a.H>b.H;
			else continue;
		} 
		if(H[i]=='R'){
			if(a.R!=b.R) return a.R>b.R;
			else continue;
		} 
		if(H[i]=='S'){
			if(a.S!=b.S) return a.S>b.S;
			else continue;
		} 
	}
	return a.name<b.name;
}

bool cmp_R(student a, student b){
	for(int i = 0;i<4;i++){
		if(R[i]=='G'){
			if(a.G!=b.G) return a.G>b.G;
			else continue;
		} 
		if(R[i]=='H'){
			if(a.H!=b.H) return a.H>b.H;
			else continue;
		} 
		if(R[i]=='R'){
			if(a.R!=b.R) return a.R>b.R;
			else continue;
		} 
		if(R[i]=='S'){
			if(a.S!=b.S) return a.S>b.S;
			else continue;
		} 
	}
	return a.name<b.name;
}

bool cmp_S(student a, student b){
	for(int i = 0;i<4;i++){
		if(S[i]=='G'){
			if(a.G!=b.G) return a.G>b.G;
			else continue;
		} 
		if(S[i]=='H'){
			if(a.H!=b.H) return a.H>b.H;
			else continue;
		} 
		if(S[i]=='R'){
			if(a.R!=b.R) return a.R>b.R;
			else continue;
		} 
		if(S[i]=='S'){
			if(a.S!=b.S) return a.S>b.S;
			else continue;
		} 
	}
	return a.name<b.name;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	cin>>G>>H>>R>>S;
	vector<student> Gryffindor,Hufflepuff,Ravenclaw,Slytherin;
	while(num--){
		student tmp;
		int max = 0;
		cin>>tmp.name>>tmp.G;
		// find what house the student is in
		if(tmp.G>max) max = tmp.G;
		cin>>tmp.H;
		if(tmp.H>max) max = tmp.H;
		cin>>tmp.R;
		if(tmp.R>max) max = tmp.R;
		cin>>tmp.S;
		if(tmp.S>max) max = tmp.S;
		// house distribution
		if(max==tmp.G){
			Gryffindor.push_back(tmp);
		}
		else if(max==tmp.H){
			Hufflepuff.push_back(tmp);
		}
		else if(max==tmp.R){
			Ravenclaw.push_back(tmp);
		}
		else{
			Slytherin.push_back(tmp);
		}
	}
	//list students
	cout<<"GRYFFINDOR:";
	if(Gryffindor.empty()) cout<<" NO NEW STUDENTS";
	cout<<'\n';
	sort(Gryffindor.begin(),Gryffindor.end(),cmp_G);
	for(auto &i : Gryffindor){
		cout<<i.name<<'\n';
	}
	cout<<"HUFFLEPUFF:";
	if(Hufflepuff.empty()) cout<<" NO NEW STUDENTS";
	cout<<'\n';
	sort(Hufflepuff.begin(),Hufflepuff.end(),cmp_H);
	for(auto &i : Hufflepuff){
		cout<<i.name<<'\n';
	}
	cout<<"RAVENCLAW:";
	if(Ravenclaw.empty()) cout<<" NO NEW STUDENTS";
	cout<<'\n';
	sort(Ravenclaw.begin(),Ravenclaw.end(),cmp_R);
	for(auto &i : Ravenclaw){
		cout<<i.name<<'\n';
	}
	cout<<"SLYTHERIN:";
	if(Slytherin.empty()) cout<<" NO NEW STUDENTS";
	cout<<'\n';
	sort(Slytherin.begin(),Slytherin.end(),cmp_S);
	for(auto &i : Slytherin){
		cout<<i.name<<'\n';
	}
}
