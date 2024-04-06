#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
  	vector<int> v;
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    int q;
    char c;
    cin>>q;
    while(q--)
    {
        cin>>c;
        if(c== 'a')
        {
            int add;
            cin>>add;
            v.push_back(add);
        }
        else if(c== 'e')
        {
            int ele;
            cin>>ele;
            auto p = find(v.begin(),v.end(),ele);
            while(p!=v.end())
            {
                v.erase(p);
                p = find(v.begin(),v.end(),ele);
            }
        }
        else if(c== 'r')
        {
            reverse(v.begin(),v.end());
        }
        else if(c== 'd')
        {
            v.pop_back();
        }
        else if(c== 's')
        {
            cout<<v.size()<<" ";
        }
        else if(c== 'p')
        {
            for(auto &i:v)
            {
                cout<<i<<" ";
            }
        }
    }
    return 0;
}
