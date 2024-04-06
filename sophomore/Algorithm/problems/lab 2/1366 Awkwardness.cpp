#include<iostream>
using namespace std;

int num; // also serve as a pointer to traverse the post[] to find nodes
int awkwardness = 0;
int age[2000006],in[2000006],post[2000006],in_index[2000006]; // in_index record permutation to index

int traverse(int start, int end){ // start and end index in in[].
	// stop at start==end
	// search post[last--]'s index in in_map => in_index
	// partition in[] into build_left(start,in_index-1) and build_right(in_index,end-1)
	// cur = (post[cur_last],build_left,build_right)
	
	if(start>end){ // it itself is a nullptr 
		return 0;
	}
	
	num--; // decrement when a new node is built
		
	if(start==end){ // it is a node without child
		return age[in[start]-1];
	}
	int cur = age[post[num]-1]; // post[num] is the current node
	int index = in_index[post[num]];
	
	// right should be traversed first as we build the tree from right to left
	if( traverse(index+1,end) > cur) awkwardness++;
	if( traverse(start,index-1) > cur) awkwardness++;
	return cur;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	cin>>num;
	for(int i = 0;i<num;i++){
		cin>>age[i];
	}
	for(int i = 0;i<num;i++){
		cin>>in[i];
		in_index[in[i]] = i;
	}
	for(int i = 0;i<num;i++){
		cin>>post[i];
	}
	traverse(0,num-1);
	cout<<awkwardness;
}
