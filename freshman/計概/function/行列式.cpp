#include<stdio.h>
int mat[10][10];
int all;

int det(int A[],int B[],int line)
{
	if(line==1)
	{
		return mat[A[0]][B[0]];
	}
	long long res=0;
	int nA[line-1],nB[line-1];
	for(int i = 0;i<line-1;i++)
	{
		nA[i]=A[i+1];
	}
	for(int i = 0;i<line;i++)
	{
		for(int j = 0;j<line;j++)
		{
			if(i>j) nB[j]=B[j];
			else nB[j]=B[j+1];
		}
		int sign=1;
		if(i&1) sign=-1;
		long long down=det(nA,nB,line-1);
		res += sign*mat[A[0]][B[i]]*down;
	}
	return res;
}

int main()
{
	int line;
	while(scanf("%d",&all)!=EOF)
	{
		int A[all],B[all];
		for(int i = 0;i<all;i++)
		{
			for(int j = 0;j<all;j++)
			{
				scanf("%d",&mat[i][j]);
			}
		}
		for(int i = 0;i<all;i++)
		{
			A[i]=i,B[i]=i;
		}
		long long ans;
		ans=det(A,B,all);
		if(ans<0) ans+=100000007;
		printf("%lld\n",ans);
	}
}
