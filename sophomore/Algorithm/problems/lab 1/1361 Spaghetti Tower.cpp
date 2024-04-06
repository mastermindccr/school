#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	int lengths;
	long long segments;
	cin>>lengths>>segments;
	vector<long long> vec(lengths);
	for(int i = 0;i<lengths;i++){
		cin>>vec[i];
	}
	sort(vec.begin(),vec.end());
	int num_query;
	cin>>num_query;
	while(num_query--){
		long long length_cut,length_use;
		cin>>length_cut>>length_use;
		long long max_length = std::min(length_cut*segments,length_use); // max length to cut
		long long spaghetti = 0;
		long long times_iter = 0;
		while(true){
			times_iter++;
			auto start = lower_bound(vec.begin(),vec.end(),length_cut*times_iter);
			long long finish_num = std::min(length_cut*(times_iter+1)-1,max_length);
			auto finish = upper_bound(start,vec.end(),finish_num);
			spaghetti += times_iter*(finish-start);
			if(finish_num >= max_length) break;
		}
		cout<<spaghetti<<'\n';
	}
}
