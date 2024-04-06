#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

typedef struct {
	int d;
	int e;
	int p;
	int s; // min shield
} status;

vector<status> warrior;
vector<status> stage;

int gcd(int a, int b){
	while(b!=0){
		int tmp = a%b;
		a = b;
		b = tmp;
	}
	return a;
}

bool simulate(int w, int is){ // w'th warrior, O(s) to run
	if(warrior[w].s<=is) return true;
	int s = is;
	int dw = warrior[w].d;
	ll ew = (ll)warrior[w].e;
	int pw = warrior[w].p;

	for(auto &i: stage){
		int aff = gcd(pw, i.p);
		int ds = i.d + aff;
		int es = i.e + aff;
		int a = ceil((double)es/dw)-1; // times being attacked
		int f = max(s-ds+1, 0); // times that fully block dmg
		if(f>=a) s -= a; // all attacks can be fully absorbed
		else{
			if(s>=ds){ // some of the attacks can be fully absorbed
				ew -= (ll)(a-f)*(a-f+1)/2;
				if(s<a){ // no shield left
					ew -= (ll)(a-s)*ds;
				}
			}
			else{ // no attack can be fully absorbed
				ew -= (ll)ds*a;
				if(s>=a){ // all attacks can be partially absorbed
					ew += (ll)a*(s+s-a+1)/2;
				}
				else{ // no shield left
					ew += (ll)s*(s+1)/2;
				}
			}
			s = max(s-a, 0);
			if(ew<=0){ // lose
				return false;
			}
		}
	}
	warrior[w].s = is;
	return true;
}

int main(){
	cin.tie(0);
	cin.sync_with_stdio(0);
	int n, m;
	cin>>n>>m;
	warrior.resize(n);
	stage.resize(m);
	for(int i = 0;i<n;i++){
		cin>>warrior[i].d;
		warrior[i].s = (int)1e9;
	}
	for(int i = 0;i<n;i++){
		cin>>warrior[i].e;
	}
	for(int i = 0;i<n;i++){
		cin>>warrior[i].p;
	}
	for(int i = 0;i<m;i++){
		cin>>stage[i].d;
	}
	for(int i = 0;i<m;i++){
		cin>>stage[i].e;
	}
	for(int i = 0;i<m;i++){
		cin>>stage[i].p;
	}
	
	int left = -1; // margin case: 0
	int right = (int)1e9;
	while(left+1<right){
		bool fail = false;
		int mid = (left+right)/2;
		for(int i = 0;i<n;i++){
			if(!simulate(i, mid)){
				left = mid;
				fail = true;
				break;
			}
		}
		if(!fail){
			right = mid;
		}
	}
	cout<<right<<'\n';
}
