#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class node{
public:
    node(int v,node* a,node* b):val(v),leftptr(a),rightptr(b){}
    int val;
    node* leftptr;
    node* rightptr;
};

void printnum(node* a){
    if(a->leftptr!=nullptr) printnum(a->leftptr);
    if(a->rightptr!=nullptr) printnum(a->rightptr);
    cout<<a->val<<endl;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int num;
    vector<node*> vec;
    while(cin>>num){
        node* a = new node(num,nullptr,nullptr);
        vec.push_back(a);
        node* cmp = vec[0];
        while(vec.size()!=1){
            if(a->val<cmp->val) {
                if(cmp->leftptr!=nullptr)
                    cmp = cmp->leftptr;
                else{
                    cmp->leftptr = a;
                    break;
                }
            }
            else{
                if(cmp->rightptr!=nullptr)
                    cmp = cmp->rightptr;
                else{
                    cmp->rightptr = a;
                    break;
                }
            }
        }
    }
    printnum(vec[0]);
    return 0;
}

