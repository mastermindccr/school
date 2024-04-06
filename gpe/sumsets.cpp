#include<iostream>
#include<vector>
using namespace std;

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int N;
	while(cin>>N){
		if(N==0) break;
		vector<int> vec;
		for(int i = 0;i<N;i++){
			int tmp;
			cin>>tmp;
			if(vec.size()==0){
				vec.push_back(tmp);
			}
			else{
				if(tmp<vec[0]){
					vec.insert(vec.begin(),tmp);
				}
				else if(tmp>vec[vec.size()-1]){
					vec.push_back(tmp);
				}
				else{
					int left = 0;
					int right = vec.size()-1;
					while(true){
						int mid = (left+right)/2;
						if(vec[mid]>tmp) right = mid;
						else if(vec[mid+1]<tmp) left = mid;
						else{
							vec.insert(vec.begin()+mid+1,tmp);
							break;
						}
					}
				}
			}
		}
		if(N<4){
			cout<<"no solution"<<endl;
		}
		else{
			bool finish = false;
			for(auto i = vec.end()-1;i!=vec.begin()+2;i--){
				for(auto j = vec.begin();j!=i-2;j++){
					for(auto k = j+1;k!=i-1;k++){
						for(auto l = k+1;l!=i;l++){
							int tmp_i = *i, tmp_j = *j, tmp_k = *k, tmp_l = *l;
							if(tmp_i < tmp_j + tmp_k + tmp_l) break;
							if(tmp_i == tmp_j + tmp_k + tmp_l){
								cout<<tmp_i<<endl;
								finish = true;
								break;
							}
						}
						if(finish) break;
					}
					if(finish) break;
				}
				if(finish) break;
			}
			if(!finish) cout<<"no solution"<<endl;
		}
	}	
}
