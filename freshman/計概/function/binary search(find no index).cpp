#include<stdio.h>

void arrange(int left,int right,int ans,int arr[])
{
	if(right-left==1)
	{
		if(arr[right] == ans)
		{
			printf("the ans is at index %d",right);
			return;
		}
		if(arr[left] == ans)
		{
			printf("the ans is at index %d",left);
			return;
		}
		printf("could not find the number %d",ans);
		return;
	}
	if(arr[(left+right)/2] == ans)
	{
		printf("the ans is at index %d",(left+right)/2);
		return;
	}
	if(arr[(left+right)/2]>ans)
	{
		right = (left+right)/2;
	}
	else
	{
		left = (left+right)/2;
	}
	arrange(left,right,ans,arr);
}

int main()
{
	int a,ans;
	scanf("%d",&a);
	int b[10];
	for(int i = 0;i<a;i++)
	{
		scanf("%d",&b[i]);
	}
	scanf("%d",&ans);
	arrange(0,a-1,ans,b);
}
