#include<stdio.h>
int yes;
int have[100];
int array[10000][100];
void arrange(int pos,int sum,int ans,int all,int arr[])
{
	if(sum == ans)
	{
		for(int i = 2;i<=all;i++)
		{
			if(!have[i]) printf("X");
			else (printf("O"));
		}
		yes = 1;
		printf("\n");
		return;
	}
	if(sum > ans) return;
	for(int i = pos;i<=all;i++)
	{
		if(!have[i])
		{
			have[i] = 1;
			sum+=arr[i];
			arrange(i+1,sum,ans,all,arr);
			have[i] = 0;
			sum-=arr[i];
		}
	}
}

int main()
{
	int a;
	scanf("%d",&a);
	for(int i = 0;i<a;i++)
	{
		int j = 1;
		while(scanf("%d",&array[i][j])!=EOF)
		{
			if(array[i][j]==0) break;
			j++;
			array[i][0]++;
		}
	}
	for(int i = 0;i<a;i++)
	{
		yes = 0;
		arrange(2,0,array[i][1],array[i][0],array[i]);
		if(yes == 1) printf("finish\n");
		else printf("no solution\n");
	}
}
