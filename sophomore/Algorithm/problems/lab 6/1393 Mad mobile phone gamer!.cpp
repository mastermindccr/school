#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int N,M,A,B,W,K,F,T;

void floyd_warshall(ll mat[505][505]){
	for(int k = 0;k<N;k++){
		for(int i = 0;i<N;i++){
			for(int j = 0;j<N;j++){
				if((ll)mat[i][k]+mat[k][j]<mat[i][j]) mat[i][j] = (ll)mat[i][k]+mat[k][j]; 
			}
		}
	}
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>N>>M;
	ll mat[505][505];
	for(int i = 0;i<N;i++){
		for(int j = 0;j<N;j++){
			if(i==j) mat[i][j] = 0;
			else mat[i][j] = (ll)1e18;
		}
	}
	while(M--){
		cin>>A>>B>>W;
		if(W<mat[A][B]) mat[A][B] = W;
	}
	floyd_warshall(mat);
	cin>>K;
	while(K--){
		cin>>F>>T;
		if(mat[F][T] == (ll)1e18) cout<<-1<<'\n';
		else cout<<mat[F][T]<<'\n';
	}
} 
