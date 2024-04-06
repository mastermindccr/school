#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num,target;
	cin>>num>>target;
	vector<int> stones(num);
	for(int i = 0;i<num;i++){
		cin>>stones[i];
	}
	sort(stones.begin(),stones.end());
	int solutions = 0;
	int last_cur = -101010101;
	for(int i = 0;i<num-1;i++){
		int cur = target-stones[i];
		if(last_cur==cur) continue;
		int beg = i+1;
		int last = num-1;
		int last_tmp = 101010101;
		while(beg!=last){
			int tmp = stones[beg]+stones[last];	
			if(tmp==last_tmp){ // same solution
				beg++;
			}
			else if(tmp<cur){ // sum is smaller than the target
				beg++;
			}
			else if(tmp>cur){ // sum is greater than the target
				last--;
			}
			else{ //sum is equal to the target
				solutions++;
				beg++;
			}
			last_tmp = tmp;
		}
		last_cur = cur;
	}
	if(solutions==0) cout<<"FeiDooDoo_Zuo_Wei_Men";
	else cout<<solutions;
}
