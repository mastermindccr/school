#include<iostream>
#include<vector>
using namespace std;

int main(){
	int N;
	while(cin>>N){
		if(N==0) break;
		bool finish = false;
		for(int m=1;m<=N;m++){
			vector<int> lists;
			for(int i=2;i<=N;i++){
				lists.push_back(i);
			}
			int count = 1;
			int cur = 0;
			while(true){
				if(count!=m){
					lists.push_back(lists[cur]);
					count++;
				}
				else{
					if(lists[cur] == 13){
						if(cur == lists.size()-1){
							cout<<m<<endl;
							finish = true;
						}
						break;
					}
					count = 1;
				}
				cur++;
			}
			if(finish) break; 
		}
	}	
}
