#include<stdio.h>
long long a,b,c;
long long mod(long long a,long long b,long long c)
{
	long long tmp = (a%c);
	long long ans = 1;
	while(b)
	{
		if(b&1)
			ans=(ans*tmp)%c;
		b/=2;
		tmp=(tmp*tmp)%c;
	}
	return ans;
}
int main()
{
	while(scanf("%lld %lld %lld",&a,&b,&c)!=EOF)
	{
		printf("%lld\n",mod(a,b,c));
	}
} 
