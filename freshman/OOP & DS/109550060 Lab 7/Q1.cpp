#include <iostream>

using namespace std;

struct Node {
    int val;
    Node *next;
    Node() : val(0), next(nullptr) {}
    Node(int x) : val(x), next(nullptr) {}
};

class LinkedList {
public:
    Node *root;
    int len;
    LinkedList() : root(nullptr), len(0) {}
    void push(int);
    void insert(int, int);
    int find(int);
    void remove(int);
    void print();
};

void LinkedList::push(int x){
    // your implementation
    if(len==0){
        root = new Node(x);
    }
    else{
        Node* cur = root;
        for(int i = 0;i<len-1;i++){
            cur = cur->next;
        }
        cur->next = new Node(x);
    }
    len++;
}

void LinkedList::insert(int pos, int x){
    // your implementation
    if(pos>len ||pos<0){
        cout<<"insert fail"<<endl;
    }
    else if(len==0){
        root = new Node(x);
        len++;
    }
    else if(pos==0){
        Node* temp = root;
        root = new Node(x);
        root->next = temp;
        len++;
    }
    else{
        Node* cur = root;
        Node* temp = new Node(x);
        Node* save = new Node;
        for(int i = 0;i<pos-1;i++){
            cur = cur->next;
        }
        if(cur->next==nullptr){
            cur->next = new Node(x);
        }
        else{
            save = cur->next;
            temp->next = save;
            cur->next = temp;
        }
        len++;
    }
}

int LinkedList::find(int x){
    // your implementation
    Node* cur = root;
    for(int i = 0;i<len;i++){
        if(cur->val == x)
            return i;
        if(cur->next !=nullptr){
            cur = cur->next;
        }
    }
    return -1;
}

void LinkedList::remove(int pos){
    // your implementation
    if(pos>=len || pos<0){
        cout<<"remove fail"<<endl;
    }
    else if(pos==0){
        root = root->next;
        len--;
    }
    else{
        Node* cur = root;
        for(int i = 1;i<pos;i++){
            cur = cur->next;
        }
        Node* temp = cur->next;
        if(temp->next!=nullptr){
            temp = temp->next;
            cur->next = temp;
        }
        else{
            cur->next = nullptr;
        }
        len--;
    }
}

void LinkedList::print(){
    if(root == nullptr)
        return;

    Node *cur = root;
    while(cur->next != nullptr){
        cout << cur->val << ", ";
        cur = cur->next;
    }
    cout << cur->val << endl;
}

int main(){
    LinkedList mylist;
    
    int n, a, b;
    cin >> n;
    char c;
    for(int i = 0; i < n; i++){
        cin >> c;
        if(c == 'p'){
            cin >> a;
            mylist.push(a);
        }else if(c == 'i'){
            cin >> a >> b;
            mylist.insert(a, b);
        }else if(c == 'r'){
            cin >> a;
            mylist.remove(a);
        }else if(c == 'f'){
            cin >> a;
            cout << mylist.find(a) << endl;
        }else if(c == '?'){
            mylist.print();
        }
    }
    return 0;
}
