#include<stdio.h>
long long coe1,coe2,coe3;
long long I[3][3]={{1,0,0},{0,1,0},{0,0,1}};
long long res[3][3];

void mul(long long result[3][3],long long a[3][3],long long b[3][3])
{
	for(int i = 0;i<3;i++)
	{
		for(int j = 0;j<3;j++)
		{
			result[i][j]=0;
			for(int k = 0;k<3;k++)
			{
				result[i][j]+=(a[i][k]%10000007*b[k][j]%10000007)%10000007;
			}
		}
	}
}

void cpy(long long dest[3][3],long long beg[3][3])
{
	for(int i = 0;i<3;i++)
	{
		for(int j = 0;j<3;j++)
		{
			dest[i][j] = beg[i][j];
		}
	}
}

void pow(long long test)
{
	long long a[3][3];
	long long tmp[3][3];
	long long trans[3][3]={{coe1,coe2,coe3},{1,0,0},{0,1,0}};
	cpy(res,I);
	cpy(a,trans);
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
	long long num,a,b,c,n;
	scanf("%lld",&num);
	while(num--)
	{
		scanf("%lld %lld %lld %lld %lld %lld %lld",&a,&b,&c,&coe1,&coe2,&coe3,&n);
		pow(n);
		printf("%lld\n",(res[2][0]*c+res[2][1]*b+res[2][2]*a)%10000007);
	}
} 
