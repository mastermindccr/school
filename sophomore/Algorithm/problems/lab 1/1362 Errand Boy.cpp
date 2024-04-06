#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	int n,l,k;
	cin>>n>>l>>k;
	vector<int> books(n); 
	for(int i = 0;i<n;i++){
		cin>>books[i];
	}
	sort(books.begin(),books.end());
	int times = 0;
	int left = 0;
	int right = n-1;
	while(left<=right){
		if(books[left]+books[right]<=l){
			left++;
			right--;
		}
		else{
			right--;
		}
		times++;
	}
	cout<<(long long)k*times*2;
}
