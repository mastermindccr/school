#include<iostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;

int main(){
	string input;
	getline(cin,input);
	stringstream ss(input);
	vector<string> arr;
	string temp;
	while(ss>>temp){
		arr.push_back(temp);
	}
	vector<string> tmp;
	for(auto &i : arr){
		if(!isdigit(i[0])){ // when encounter an operator means there's something to calculate
			long long a = stoi(*(tmp.end()-2)) % 1000000007;
			long long b = stoi(*(tmp.end()-1)) % 1000000007;
			long long result;
			tmp.pop_back();
			tmp.pop_back(); // erase the last two numbers
			if(i == "+"){
				tmp.push_back(to_string((a+b) % 1000000007));
			}
			else{
				tmp.push_back(to_string((a*b) % 1000000007));
			}
		}
		else{
			tmp.push_back(i);
		}
	}
	cout<<tmp[0];
}
