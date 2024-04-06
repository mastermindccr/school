#include<stdio.h>

long long trans[2][2]={{1,1},{1,0}};
long long I[2][2]={{1,0},{0,1}};
long long N[2][2]={{0,0},{0,0}};
long long res[2][2];

void cpy(long long dest[2][2],long long beg[2][2])
{
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<2;j++)
		{
			dest[i][j] = beg[i][j];
		}
	} 
}

void mul(long long result[2][2],long long a[2][2],long long b[2][2])
{
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<2;j++)
		{
			result[i][j] = 0;
			for(int k = 0;k<2;k++)
			{
				result[i][j]+=a[i][k]*b[k][j];
				result[i][j]%=10000007;
			}
		}
	}
}

void pow(long long test)
{
	long long a[2][2];
	long long tmp[2][2];
	cpy(a,trans);
	cpy(res,I);
	while(test)
	{
		if(test&1)
		{
			cpy(tmp,res);
			mul(res,tmp,a);
		}
		test/=2;
		cpy(tmp,a);
		mul(a,tmp,tmp);
	}
}
int main()
{
	int num;
	long long test;
	scanf("%d",&num);
	while(num--)
	{
		scanf("%lld",&test);
		pow(test);
		printf("%lld\n",res[1][0]);
	}
}
