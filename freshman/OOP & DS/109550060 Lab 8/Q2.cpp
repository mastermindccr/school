#include <iostream>
#include <queue>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    queue<int> arr;
    int num;
    int buf;
    cin>>num;
    while(num--){
        cin>>buf;
        arr.push(buf);
        while(arr.front()+5000<buf){
            arr.pop();
        }
        cout<<arr.size()<<" ";
    }
    return 0;
}
