#include<stdio.h>

int main()
{
	int a,b;
	char s;
	scanf("%d %d",&a,&b);
	int arr[a];
	int arr2[b];
	for(int i = 0;i<a;i++)
	{
		scanf("%d%c",&arr[i],&s);
	}
	for(int i = 0;i<b;i++)
	{
		scanf("%d%c",&arr2[i]);
	}
	for(int i = 0;i<b;i++)
	{
		int left = 0;
		int right = a;
		while(arr[(left+right)/2]!=arr2[i])
		{
			(arr[(left+right)/2]>arr2[i])?right = (left+right)/2:left = (left+right) /2;
		}
		printf("%d\n",(left+right)/2);
	}
}
