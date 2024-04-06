#include<iostream>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int col,row;
	cin>>col>>row;
	long long arr[row][col+2];
	for(int i = 0;i<row;i++){
		for(int j = 0;j<col+2;j++){
			if(j==0 || j==col+1){
				arr[i][j] = -10000000000000;
			}
			else{
				cin>>arr[i][j];
			}
		}
	}
	for(int i = row-2;i>=0;i--){
		for(int j = 1;j<=col;j++){
			arr[i][j] += max(max(arr[i+1][j-1],arr[i+1][j]),arr[i+1][j+1]);
		}
	}
	for(int j = 1;j<=col;j++){
		cout<<arr[0][j]<<' ';
	}
}
