#include<stdio.h>
int have[8];
int now[8];
void arrange(int pos,int all,int arr[]);

int main()
{
	int num;
	int b[8];
	scanf("%d",&num);
	for(int i = 0;i<num;i++)
	{
		scanf("%d",&b[i]);
	}
	for(int i = 0;i<num;i++)
	{
		for(int j = 0;j<b[i];j++)
		{
			have[j] = 0;
		}
		arrange(0,b[i],now);
	}
}

void arrange(int pos,int all,int arr[])
{
	if(pos==all)
	{
		for(int i = 0;i<all;i++)
		{	
			printf("%d",arr[i]);
		}
		printf("\n");
		return;
	}
	for(int i = 1;i<=all;i++)
	{
		if(!have[i])
		{
			have[i] = 1;
			arr[pos] = i;
			arrange(pos+1,all,arr);
			have[i] = 0;
		}
	}
}
