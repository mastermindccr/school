#include<stdio.h>
int all;
int array[12];
int count[12];
int have;

void arrange(int pos,int sum)
{
	if(sum==array[0])
	{
		for(int i = 1;i<all;i++)
		{
			for(int j = 0;j<count[i];j++)
			{
				printf("%d ",array[i]);
			}
		}
		have = 1;
		printf("\n");
		return;
	}
	if(sum>array[0]) return;
	for(int i = pos;i<all;i++)
	{
		sum+=array[i];
		count[i]++;
		arrange(i,sum);
		sum-=array[i];
		count[i]--;
	}
} 

int main()
{
	int num;
	scanf("%d",&num);
	for(int i = 0;i<num;i++)
	{
		have = 0;
		all = 0;
		while(scanf("%d",&array[all]))
		{
			if(array[all]==0) break;
			all++;
		}
		arrange(1,0);
		if(have) printf("finish\n");
		else printf("no solution\n");
	}
	
} 
