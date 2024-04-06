#include <bits/stdc++.h>

using namespace std;

typedef struct {
	int x;
	int y;
	int val;
} grid;

bool cmp(grid &a, grid &b){
	return a.val<b.val;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	
	// calculate outer product of (u X v), fit A with (u X v)
	int n, m;
	cin>>n>>m;
	vector<int> u(n);
	vector<int> v(m);
	vector<int> inp_a(n*m);
	vector<vector<int>> a(n, vector<int>(m));
	vector<vector<int>> prod(n, vector<int>(m));
	vector<grid> grids;
	
	for(auto &i: u){
		cin>>i;
	}
	for(auto &i: v){
		cin>>i;
	}
	for(auto &i: inp_a){
		cin>>i;
	}
	
	for(int i = 0;i<n;i++){
		for(int j = 0;j<m;j++){
			prod[i][j] = u[i]*v[j];
			grids.push_back({i, j, u[i]*v[j]});
		}
	}
	sort(grids.begin(), grids.end(), cmp);
	sort(inp_a.begin(), inp_a.end());
	long long val = 0;
	for(int i = 0;i<n*m;i++){
		int x = grids[i].x;
		int y = grids[i].y;
		a[x][y] = inp_a[i];
		val += (long long)inp_a[i]*grids[i].val;
	}
	cout<<val<<'\n';
	for(auto &i: u){
		cout<<i<<' ';
	}
	cout<<'\n';
	for(auto &i: v){
		cout<<i<<' ';
	}
	cout<<'\n';
	for(auto &i: a){
		for(auto &j: i){
			cout<<j<<' ';
		}
		cout<<'\n';
	}
}
