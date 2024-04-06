#include<iostream>

using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	int LIS[N]; // record the increasing array
	int LDS[N]; // record the decreasing array
	int LIS_length[N]; // record the max length of current index
	int LDS_length[N]; // record the max length of current index
	int arr[N];
	int ans = 0;
	// do /\ form first
	// initialize
	for(int i = 0;i<N;i++){
		LIS[i] = LDS[i] = LIS_length[i] = LDS_length[i] = 0;
		cin>>arr[i];
	}
	// do LIS part
	LIS[0] = arr[0];
	LIS_length[0] = 1;
	
	for(int i = 1;i<N;i++){
		if(arr[i]<LIS[0]){ // if current number is smaller than the first LIS number
			LIS[0] = arr[i];
			LIS_length[i] = LIS_length[i-1];
		}
		else if(arr[i]>LIS[LIS_length[i-1]-1]){ // if current number is larger than last LIS number
			LIS_length[i] = LIS_length[i-1]+1;
			LIS[LIS_length[i]-1] = arr[i];
		}
		else{
			LIS_length[i] = LIS_length[i-1];
			int left = 0;
			int right = LIS_length[i]-1;
			int mid;
			// tmp>mid and tmp<mid+1
			while(true){
				mid = (left+right)/2;
				if(arr[i] == LIS[mid] || arr[i] == LIS[mid+1]) break;
				if(arr[i]>LIS[mid+1]) left = mid;
				else if(arr[i]<LIS[mid]) right = mid;
				else{
					LIS[mid+1] = arr[i];
					break;
				}
			}
		}
	}
	// traverse the array from back and to LDS part
	LDS[0] = arr[N-1];
	LDS_length[N-1] = 1;
	for(int i = N-2;i>=0;i--){
		if(arr[i]<LDS[0]){
			LDS[0] = arr[i];
			LDS_length[i] = LDS_length[i+1];
		}
		else if(arr[i]>LDS[LDS_length[i+1]-1]){
			LDS_length[i] = LDS_length[i+1]+1;
			LDS[LDS_length[i]-1] = arr[i];
		}
		else{
			LDS_length[i] = LDS_length[i+1];
			int left = 0;
			int right = LDS_length[i]-1;
			int mid;
			// tmp>mid and tmp<mid+1
			while(true){
				mid = (left+right)/2;
				if(arr[i] == LDS[mid] || arr[i] == LDS[mid+1]) break;
				if(arr[i]>LDS[mid+1]) left = mid;
				else if(arr[i]<LDS[mid]) right = mid;
				else{
					LDS[mid+1] = arr[i];
					break;
				}
			}
		}	
	}
	for(int i = 0;i<N;i++){
		if(LIS_length[i]+LDS_length[i]-1>ans) ans = LIS_length[i]+LDS_length[i]-1;
	}
	// then \/ form
	// initialize
	for(int i = 0;i<N;i++){
		LIS[i] = LDS[i] = LIS_length[i] = LDS_length[i] = 0;
	}
	// do LIS part
	LIS[0] = arr[0];
	LIS_length[0] = 1;
	
	for(int i = 1;i<N;i++){
		if(arr[i]>LIS[0]){ // if current number is larger than the first LIS number
			LIS[0] = arr[i];
			LIS_length[i] = LIS_length[i-1];
		}
		else if(arr[i]<LIS[LIS_length[i-1]-1]){ // if current number is smaller than last LIS number
			LIS_length[i] = LIS_length[i-1]+1;
			LIS[LIS_length[i]-1] = arr[i];
		}
		else{
			LIS_length[i] = LIS_length[i-1];
			int left = 0;
			int right = LIS_length[i]-1;
			int mid;
			// tmp>mid and tmp<mid+1
			while(true){
				mid = (left+right)/2;
				if(arr[i] == LIS[mid] || arr[i] == LIS[mid+1]) break;
				if(arr[i]<LIS[mid+1]) left = mid;
				else if(arr[i]>LIS[mid]) right = mid;
				else{
					LIS[mid+1] = arr[i];
					break;
				}
			}
		}
	}
	// traverse the array from back and to LDS part
	LDS[0] = arr[N-1];
	LDS_length[N-1] = 1;
	for(int i = N-2;i>=0;i--){
		if(arr[i]>LDS[0]){
			LDS[0] = arr[i];
			LDS_length[i] = LDS_length[i+1];
		}
		else if(arr[i]<LDS[LDS_length[i+1]-1]){
			LDS_length[i] = LDS_length[i+1]+1;
			LDS[LDS_length[i]-1] = arr[i];
		}
		else{
			LDS_length[i] = LDS_length[i+1];
			int left = 0;
			int right = LDS_length[i]-1;
			int mid;
			// tmp>mid and tmp<mid+1
			while(true){
				mid = (left+right)/2;
				if(arr[i] == LDS[mid] || arr[i] == LDS[mid+1]) break;
				if(arr[i]<LDS[mid+1]) left = mid;
				else if(arr[i]>LDS[mid]) right = mid;
				else{
					LDS[mid+1] = arr[i];
					break;
				}
			}
		}	
	}
	for(int i = 0;i<N;i++){
		if(LIS_length[i]+LDS_length[i]-1>ans) ans = LIS_length[i]+LDS_length[i]-1;
	}
	cout<<ans;
}
