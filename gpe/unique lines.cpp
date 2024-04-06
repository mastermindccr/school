#include<iostream>
#include<vector>
#include<utility>
#include<unordered_map>

using namespace std;

int main(){
	int N;
	cin>>N;
	while(N--){
		int num_lines = 0;
		int num_points;
		cin>>num_points;
		vector<pair<int,int>> points;
		while(num_points--){
			unordered_map<double,int> slopes; // slope and its corresponding times of appearance
			int fir, sec;
			cin>>fir>>sec;
			for(auto &i : points){ // compare with every points before
				double cur_slope = (double)(sec-i.second)/(double)(fir-i.first);
				if(slopes.find(cur_slope) != slopes.end()){ // slope exists
					slopes[cur_slope]++;
				}
				else{
					slopes.insert(make_pair(cur_slope,1));
				}
			}
			for(auto &i : slopes){
				if(i.second == 1){ // it is a unique line
					num_lines++;
				}
			}
			points.push_back(make_pair(fir,sec));
		}
		cout<<num_lines<<endl;
	}
	// record every pair
	// input new pair
	// record slope and the corresponding point
}
