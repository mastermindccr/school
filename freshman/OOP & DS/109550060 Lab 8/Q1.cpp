#include <iostream>
#include <stack>
using namespace std;

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int num;
    cin>>num;
    while(num--){
        stack<char> arr;
        bool Y = true;
        string a;
        cin>>a;
        for(int i = 0;a[i]!='\0';i++){
            if(a[i]=='(' || a[i]=='{' || a[i]=='['){
                arr.push(a[i]);
            }
            else{
                if(!arr.empty() && ((arr.top()=='(' && a[i]==')') || (arr.top()=='[' && a[i]==']') || (arr.top()=='{' && a[i]=='}'))){
                    arr.pop();
                }
                else{
                    Y = false;
                    break;
                }
            }
        }
        if(Y && arr.empty()) cout<<"yes"<<endl;
        else cout<<"no"<<endl;
    }
    return 0;
}
