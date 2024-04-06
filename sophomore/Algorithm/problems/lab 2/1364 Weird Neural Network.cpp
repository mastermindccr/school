#include<iostream>
#include<vector>
#include<utility>
using namespace std;

struct Node{
	long long val;
	Node* left;
	Node* right;
};

void traverse(Node* n){
	if(n->left==nullptr) return; // it is a leaf node
	traverse(n->left);
	traverse(n->right);
	n->val=(n->left->val+n->right->val)*n->val%1000000007;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	cin>>N;
	vector<Node*> vec;
	for(int i = 0;i<N;i++){
		long long tmp;
		cin>>tmp;
		vec.push_back(new Node{tmp,nullptr,nullptr});
	}
	int head; // the head node of this node
	for(int i = 1;i<N;i++){
		cin>>head;
		if(vec[head-1]->left == nullptr) vec[head-1]->left = vec[i];
		else vec[head-1]->right = vec[i];
	}
	traverse(vec[0]);
	cout<<vec[0]->val;
}
