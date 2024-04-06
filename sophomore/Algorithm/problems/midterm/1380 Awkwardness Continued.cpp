#include<iostream>
#include<vector>
using namespace std;

vector<int> pre,in;
int pos = -1;

void build(int start, int end){ // start and end of in-order
	if(start > end) return;
	pos++;
	if(start==end){
		cout<<pre[pos]+1<<' ';
		return;
	}
	int tmp = pos;
	build(start,in[pre[tmp]]-1);
	build(in[pre[tmp]]+1,end);
	cout<<pre[tmp]+1<<' ';
	return;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int num;
	cin>>num;
	pre.resize(num);
	in.resize(num);
	int tmp;
	for(int i = 0;i<num;i++){
		cin>>tmp;
		pre[i] = tmp-1;
	}
	for(int i = 0;i<num;i++){
		cin>>tmp;
		in[tmp-1] = i;
	}
	build(0,num-1);
}
