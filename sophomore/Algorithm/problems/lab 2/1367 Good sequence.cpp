#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

long long same(vector<int> vec, int size){ // only pass the value as we do nth_element to find median
	if(size==1) return 0;
	long long change = 0; // num of change the vector into the same number
	nth_element(vec.begin(),vec.begin()+(size-1)/2,vec.end()); // find median
	int median = vec[(size-1)/2]; // median is the number which leads to the minimum;
	for(auto &i :vec){
		if(i<median) change += median-i;
		else change += i-median;
	}
	return change;
}

long long good(vector<int>& vec, int size){ // pass the reference to speed up
	if(size<=2) return 0;
	vector<int> front(vec.begin(), vec.begin()+(size+1)/2);
	vector<int> back(vec.begin()+(size+1)/2, vec.end());
	return min(same(front,(size+1)/2) + good(back,size/2) , good(front,(size+1)/2) + same(back,size/2));
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	vector<int> vec(N);
	for(int i = 0;i<N;i++){
		cin>>vec[i];
	}
	cout<<good(vec,N);
}
