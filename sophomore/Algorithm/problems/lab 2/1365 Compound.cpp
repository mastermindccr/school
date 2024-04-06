#include<iostream>
#include<map>
#include<cctype>
using namespace std;

map<char,int> single_map;
map<string,int> double_map;
int s_size;
string s;
int pos = 0;

void go(map<char,int>& last_single_map, map<string,int>& last_double_map){
	map<char,int> tmp_single_map;
	map<string,int> tmp_double_map;
	
	while(pos!=s_size){
		if(isupper(s[pos])){
			if(pos+1!=s_size){
				if(islower(s[pos+1])){ // upper + lower
					if(pos+2!=s_size && isdigit(s[pos+2])){ // upper + lower + digit
						string tmp_str;
						string tmp_dig;
						tmp_str.push_back(s[pos]);
						tmp_str.push_back(s[pos+1]);
						int digits = 0; // how many consecutive digits
						for(int i = pos+2;isdigit(s[i]);i++){
							digits++;
							tmp_dig.push_back(s[i]);
						}
						tmp_double_map[tmp_str]+=stoi(tmp_dig);
						pos+=digits;
					}
					else{ // upper + lower only
						string tmp;
						tmp.push_back(s[pos]);
						tmp.push_back(s[pos+1]);
						tmp_double_map[tmp]++;
					}
					pos++;
				}
				else if(isdigit(s[pos+1])){ // upper + digit
					string tmp_dig;
					int digits = 0; // how many consecutive digits
					for(int i = pos+1;isdigit(s[i]);i++){
						tmp_dig.push_back(s[i]);
						digits++;
					}
					tmp_single_map[s[pos]]+=stoi(tmp_dig);
					pos+=digits;
				}
				else{ // upper only
					tmp_single_map[s[pos]]++;
				}
			}
			else{ // upper only
				tmp_single_map[s[pos]]++;
			}
			pos++;
		}
		else if(s[pos]=='('){ // left parenthesis
			pos++;
			go(tmp_single_map,tmp_double_map);
		}
		else{ // right parenthesis
			string tmp_dig;
			int digits = 0; //how many consecutive digits
			for(int i = pos+1;isdigit(s[i]);i++){
				tmp_dig.push_back(s[i]);
				digits++;
			}
			
			for(auto &i : tmp_single_map){
				i.second*=stoi(tmp_dig);
				last_single_map[i.first]+=i.second;
			}
			for(auto &i : tmp_double_map){
				i.second*=stoi(tmp_dig);
				last_double_map[i.first]+=i.second;
			}
			pos+=digits+1;
			return;
		}	
	}
	for(auto &i : tmp_single_map){
		last_single_map[i.first]+=i.second;
	}
	for(auto &i : tmp_double_map){
		last_double_map[i.first]+=i.second;
	}
}

int main(){
	cin>>s;
	s_size = s.length();
	go(single_map, double_map);
	cout<<single_map.size()<<'\n';
	for(auto &i : single_map){
		cout<<i.first<<":"<<i.second<<'\n';
	}
	cout<<double_map.size()<<'\n';
	for(auto &i : double_map){
		cout<<i.first<<":"<<i.second<<'\n';
	}
}
