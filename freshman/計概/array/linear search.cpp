#include<stdio.h>

int main()
{
	int a,b,c;
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
		for(int j = 0;j<a;j++)
		{
			if(arr2[i]==arr[j])
			{
				printf("%d ",j);
				break;
			}
		}
	}
}
