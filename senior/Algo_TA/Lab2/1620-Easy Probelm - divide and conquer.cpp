#include<bits/stdc++.h>

using namespace std;

int mp[1024][1024];
int cnt = 1;

void fill(int i, int j, int a){
	if(a==0){
		mp[i][j] = cnt++;
		return;
	}
	fill(i, j, a/2);
	fill(i, j+a, a/2);
	fill(i+a, j, a/2);
	fill(i+a, j+a, a/2);
}

int main(){
    int n;
    cin>>n;
    int a = pow(2, n);
    fill(0, 0, a/2);
    for(int i = 0;i<a;i++){
    	for(int j = 0;j<a;j++){
    		printf("%d ", mp[i][j]);
		}
		printf("\n");
	}
}
