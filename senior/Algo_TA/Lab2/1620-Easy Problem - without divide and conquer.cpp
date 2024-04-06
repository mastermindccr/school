// directly calculate the answer instead of using divide and conquer
#include<bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin>>n;
    int a = pow(2, n);
    for(int i = 0;i<a;i++){
    	for(int j = 0;j<a;j++){
    		int ans = 0;
    		int tmp = a>>1;
    		while(tmp){
    			if(i&tmp){
    				ans += tmp*tmp*2;
				}
				if(j&tmp){
					ans += tmp*tmp;
				}
				tmp>>=1;
			}
			cout<<ans+1<<' ';
		}
		cout<<'\n';
	}
}
