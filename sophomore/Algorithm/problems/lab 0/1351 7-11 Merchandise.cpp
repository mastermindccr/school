#include<iostream>
#include<vector>
using namespace std;

struct Item{
	vector<int> index;
};

int main(){
	cin.tie(0);
  	cin.sync_with_stdio(0);
	int num_item, num_query;
	int type,number,date_or_index; //type 1 :date, type 2 :index
	cin>>num_item>>num_query;
	vector<Item> Items(num_item);
	for(int i = 0;i<num_query;i++){
		cin>>type>>number>>date_or_index;
		if(type==1){
			Items[number].index.push_back(date_or_index);
		}
		else if(type==2){
			if(Items[number].index.size()<=date_or_index){ // mention that size_t cannot be a minus so that we should avoid to minus sth
				cout<<-1;
			}
			else{
				cout<<Items[number].index[date_or_index];
			}
			cout<<'\n';
		}
	}
}
