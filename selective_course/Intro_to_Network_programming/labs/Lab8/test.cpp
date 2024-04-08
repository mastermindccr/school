#include <bits/stdc++.h>

using namespace std;

string stringify(int i){
    string ret = to_string(i);
    while(ret.length()<6){
        ret = '0'+ret;
    }
	ret = '/'+ret;
    return ret;
}

int main(){
    int correct = 0;
    for(int i = 0;i<1000;i++){
        string line1, line2;
        ifstream file1, file2;
        file1.open("files/"+stringify(i));
        file2.open("store/"+stringify(i));
        getline(file1, line1);
        getline(file2, line2);
        // cout<<line1.length()<<' '<<line2.length()<<'\n';
        if(line1==line2) correct++;
        else{
            cout<<i<<' '<<line1.length()<<' '<<line2.length()<<'\n';
        }
    }
    cout<<correct<<'\n';
}