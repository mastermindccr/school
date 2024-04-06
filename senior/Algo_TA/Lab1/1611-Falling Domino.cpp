#include <bits/stdc++.h>

using namespace std;

int main()
{
    cin.tie(0);
    cin.sync_with_stdio(0);
    long long n, m ,k;
    cin>>n>>m>>k;
    
    int wid = min(n, m);
    long long len = max(n, m);
    long long ans = 0;
    
    // rewrite binary search question into formula
    for(int i = 1;i<=wid;i++){
        long long lo = len+1-(len*wid-k+1)/(wid-i+1);
        lo = max(lo, (long long)1);
        long long hi = k/i;
        hi = min(hi, len);
        ans += (hi-lo+1)<0?0:(hi-lo+1);
    }
    
    cout<<ans<<'\n';
}
