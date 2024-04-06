#include<iostream>
#include<queue>
#include<iomanip>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	priority_queue<int> first; // max-heap
	priority_queue<int,vector<int>,greater<int>> first_left; // min_heap
	priority_queue<int,vector<int>,greater<int>> third; // min-heap
	priority_queue<int> third_left; // max-heap
	int count = 0; // the vector's size
	while(cin>>N){
		if(N==0) break;
		count++;
		// insert new element
		if(count==1){
			cout<<"0\n";
			first.push(N);
			third.push(N);
			continue;
		}
		double first_quartile,third_quartile;
		if((count/2)%2==1){ // median is the only number
			if(count==2){ // size = 2 have to rearrange the correct order
				if(first.top()>N){ // need a swap of the value in the first heap
					int tmp = first.top();
					first.pop();
					first.push(N);
					first_left.push(tmp);
					third_left.push(N);
				}
				else{ // need a swap of the value in the third heap
					int tmp = third.top();
					third.pop();
					third.push(N);
					third_left.push(tmp);
					first_left.push(N);
				}
			}
			else{ // size > 2
				if(N>third_quartile){ // the new number is larger than third quartile
					third.push(N);
					first_left.push(N);
				}
				else if(N<first_quartile){ // the new number is smaller than first quartile
					first.push(N);
					third_left.push(N);
				}
				else{ // in between first and third quartile
					first_left.push(N);
					third_left.push(N);
				}
				// balancing the two heaps
				int tmp;
				if(first.size()<count/4+1){
					tmp = first_left.top();
					first.push(tmp);
					first_left.pop();
				}
				else if(first.size()>count/4+1){
					tmp = first.top();
					first_left.push(tmp);
					first.pop();
				}
				if(third.size()<count/4+1){
					tmp = third_left.top();
					third.push(tmp);
					third_left.pop();
				}
				else if(third.size()>count/4+1){
					tmp = third.top();
					third_left.push(tmp);
					third.pop();
				}
			}
			first_quartile = (double)first.top();
			third_quartile = (double)third.top();
		}
		else{ // median is the average of the sum of two numbers
			if(N>third_quartile){ // the new number is larger than third quartile
				third.push(N);
				first_left.push(N);
			}
			else if(N<first_quartile){ // the new number is smaller than first quartile
				first.push(N);
				third_left.push(N);
			}
			else{ // in between first and third quartile
				first_left.push(N);
				third_left.push(N);
			}
			// balancing the two queues
			int tmp;
			if(first.size()<count/4){
				tmp = first_left.top();
				first.push(tmp);
				first_left.pop();
			}
			else if(first.size()>count/4){
				tmp = first.top();
				first_left.push(tmp);
				first.pop();
			}
			if(third.size()<count/4){
				tmp = third_left.top();
				third.push(tmp);
				third_left.pop();
			}
			else if(third.size()>count/4){
				tmp = third.top();
				third_left.push(tmp);
				third.pop();
			}
			first_quartile = ((double)first.top()+(double)first_left.top())/2;
			third_quartile = ((double)third.top()+(double)third_left.top())/2;
		}
		double ans = third_quartile-first_quartile;
		if(ans==(int)ans) cout<<fixed<<setprecision(0)<<ans<<'\n';
		else cout<<fixed<<setprecision(1)<<ans<<'\n';
	}
}
