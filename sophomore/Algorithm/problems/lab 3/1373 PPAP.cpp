#include<iostream>
#include<sstream>
#include<unordered_map>
#include<queue>
#include<set>
#include<iomanip>
using namespace std;
unordered_map<string,int> mp; // record the appear times of each word

struct Node{
	set<string> content;
	int val;
	Node* left;
	Node* right;
};

struct cmp{
	bool operator()(Node* a, Node* b){
		if(a->val!=b->val) return a->val>b->val;
		return a->content>b->content;
	}
};

int log(int a){
	int ans = 1;
	int times = 0;
	while(ans<a){
		times++;
		ans*=2;
	}
	return times;
}

void traverse(Node* a, int height){
	if(a->left!=nullptr){
		traverse(a->left,height+1);
		traverse(a->right,height+1);
	}
	else mp[*a->content.begin()] = height;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	priority_queue<Node*,vector<Node*>,cmp> pq; // use pq to build the tree
	vector<vector<string>> lines(N+1); // store each line of the lyrics
	
	for(int i = 0;i<=N;i++){
		string a;
		getline(cin,a);
		stringstream ss(a);
		string word;
		while(ss>>word){
			lines[i].push_back(word);
			mp[word]++;
		}
	}
	int origin_bit = log((int)mp.size());
	for(auto &i : mp){
		Node* new_node = new Node{{i.first},i.second,nullptr,nullptr};
		pq.push(new_node);
	}
	while(pq.size()!=1){
		Node* first = pq.top();
		pq.pop();
		Node* second = pq.top();
		pq.pop();
		// merge two nodes
		set<string> tmp(first->content);
		tmp.insert(second->content.begin(),second->content.end());
		Node* parent = new Node{tmp,first->val+second->val,first,second};
		pq.push(parent);
	}
	traverse(pq.top(),0);
	for(int i = 1;i<=N;i++){
		int orig = origin_bit*lines[i].size();
		int compressed = 0;
		for(auto j : lines[i]){
			compressed+=mp[j];
		}
		cout<<setprecision(8)<<(double)compressed/orig<<'\n';
	}
}
