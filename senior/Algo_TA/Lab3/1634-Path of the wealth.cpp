#include <bits/stdc++.h>
#define i128 __int128
using namespace std;

typedef struct Node{
    int id;
    i128 mult;
    i128 add;
    vector<int> order;
} Node;

struct cmp{
    bool operator()(Node &a, Node &b){
    	// caution: cannot write this as this is not a monotonous campare function
//    	if(a.id==b.id){
//    		if(a.mult==b.mult) return a.add<b.add;
//    		return a.mult<b.mult;
//		}
        if(a.mult==1) return false;
        if(b.mult==1) return true;
        
        return a.add*(b.mult-1)<b.add*(a.mult-1);
    }
};

ostream& operator << (ostream &os, const i128 &x) {
    if (x == 0) return os << "0";
    i128 tmp = x;
    int sign = (tmp > 0 ? 1 : -1);
    if (tmp < 0) tmp *= -1;
    string ret;
    while (tmp) ret += char('0' + tmp % 10), tmp /= 10;
    if (sign == -1) ret += "-";
    reverse(begin(ret), end(ret));
    return os << ret;
}

i128 inf = pow(10, 18);

int main(){
    cin.tie(0);
    cin.sync_with_stdio(0);
    int t;
    cin>>t;
    while(t--){
        int N;
        cin>>N;
        vector<int> par(N+1);
        vector<set<int>> adj(N+1);
        vector<Node> nodes(N+1);
        priority_queue<Node, vector<Node>, cmp> pq;

        for(int i = 1;i<=N;i++){
            int node;
            cin>>node;
            par[i] = node;
            adj[node].insert(i);
        }
        for(int i = 1;i<=N;i++){
            char op;
            int co;
            cin>>op>>co;
            if(op=='x'){
                nodes[i] = {i, co, 0, {i}};
            }
            else{
                nodes[i] = {i, 1, co, {i}};
            }
            pq.push(nodes[i]);
        }

        // as we always update the parent node with a better child, the updated value
        // of parent node must be better than the original value. Therefore, pq will
        // always read the new value first, we only need to use a bool array to record
        // whether the node is parsed.
        vector<bool> traversed(N+1, false);
        traversed[1] = true;
        
        while(!pq.empty()){
            Node tmp = pq.top();
            pq.pop();
			
            if(traversed[tmp.id]) continue;
            traversed[tmp.id] = true;
			
            // merge current node with the previous one
            Node par_node = nodes[par[tmp.id]];
            Node tmp_node = nodes[tmp.id]; // this is the real tmp node, as when weights are the same, older node might come first in the pq
            
            par_node.mult = min(tmp_node.mult*par_node.mult, inf);
            par_node.add = min(tmp_node.mult*par_node.add+tmp_node.add, inf);
            
            nodes[par_node.id] = par_node; // update value of parent node

            if(par_node.id!=1) pq.push(par_node);
            
            // update children of current node to it's parent using adj matrix
            for(auto &i: adj[tmp.id]){
                par[i] = par[tmp.id];
            }
            adj[par[tmp.id]].erase(tmp.id);
            adj[par[tmp.id]].insert(adj[tmp.id].begin(), adj[tmp.id].end()); // insert children of this node to its parent
            nodes[par[tmp.id]].order.insert(nodes[par[tmp.id]].order.end(), nodes[tmp.id].order.begin(), nodes[tmp.id].order.end());
        }

        i128 ans = nodes[1].mult+nodes[1].add;
        if(ans>=inf){
            cout<<"infinite"<<'\n';
        }
        else{
            cout<<ans<<'\n';
        }
        for(int i = 0;i<nodes[1].order.size();i++){
            cout<<nodes[1].order[i]<<' ';
        }
        cout<<'\n';
    }
}
