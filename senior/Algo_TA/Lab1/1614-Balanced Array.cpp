#include<bits/stdc++.h>

using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int n;
	cin>>n;
	vector<int> node(n);
	vector<int> partial(n);
	vector<int> edge(n);
	for(int i = 0;i<n;i++){
		cin>>node[i];
	}
	for(int i = 0;i<n;i++){
		cin>>edge[i];
	}
	
	// ignore the last edge(linearize the nodes) => we can use greedy from left to right to get the answer: sigma [abs(xi)*wi]
	// utilize the last edge => we can transfer a from last node to the first node in the begining => answer: sigma [abs(xi+a)*wi]
	// thus transform the question into find min of sigma [abs(xi+a)*wi], we can find the median of all the x and calculate its min
	partial_sum(node.begin(), node.end(), partial.begin());
	
	vector<pair<int, int>> res;
	
	int edge_num = 0;
	for(int i = 0;i<n;i++){
		res.push_back(make_pair(-partial[i], edge[i]));
		edge_num += edge[i];
	}
	
	sort(res.begin(), res.end());
	
	int x;
	int pos = 0;
	for(auto &i: res){
		pos += i.second;
		if(pos>=(edge_num+1)/2){
			x = i.first;
			break;
		}
	}	
	
	long long sum = 0;
	for(auto &i: res){
		sum += (long long)abs(i.first-x) * i.second;
	}
	cout<<sum<<'\n';
}
