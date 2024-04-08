#include<bits/stdc++.h>

using namespace std;

struct grid{
	int r;
	int c;
	int val;
};

struct cmp{
	bool operator()(grid a, grid b){
		return a.val>b.val;
	}
};

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int row,col;
	cin>>row>>col;
	vector<vector<int>> vec(row,vector<int>(col));
	vector<vector<bool>> pushed(row,vector<bool>(col));
	for(int i = 0;i<row;i++){
		for(int j = 0;j<col;j++){
			cin>>vec[i][j];
			pushed[i][j] = false;
		}
	}
	priority_queue<grid,vector<grid>,cmp> pq;
	for(int i = 0;i<row;i++){
		pq.push({i,0,vec[i][0]});
		pushed[i][0] = true;
	}
	int thr = 0;
	while(!pq.empty()){
		grid tmp = pq.top();
		pq.pop();
		if(thr<tmp.val){ // renew the threshold
			thr = tmp.val;
		}
		if(tmp.c==col-1) break;
		// push tmp's unpushed neighbor into pq
		if(tmp.r!=0 && !pushed[tmp.r-1][tmp.c]){
			pq.push({tmp.r-1,tmp.c,vec[tmp.r-1][tmp.c]});
			pushed[tmp.r-1][tmp.c] = true;
		}
		if(tmp.r!=row-1 && !pushed[tmp.r+1][tmp.c]){
			pq.push({tmp.r+1,tmp.c,vec[tmp.r+1][tmp.c]});
			pushed[tmp.r+1][tmp.c] = true;
		}
		if(tmp.c!=0 && !pushed[tmp.r][tmp.c-1]){
			pq.push({tmp.r,tmp.c-1,vec[tmp.r][tmp.c-1]});
			pushed[tmp.r][tmp.c-1] = true;
		}
		if(tmp.c!=col-1 && !pushed[tmp.r][tmp.c+1]){
			pq.push({tmp.r,tmp.c+1,vec[tmp.r][tmp.c+1]});
			pushed[tmp.r][tmp.c+1] = true;
		}
	}
	cout<<thr;
}
