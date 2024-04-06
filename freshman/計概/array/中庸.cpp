#include<stdio.h>

int main()
{
	int a;
	char s;
	scanf("%d",&a);
	int num[a];
	for(int i = 0;i<a;i++)
	{
		scanf("%d%c",&num[i],&s);
	}
	int max = num[0];
	for(int i = 1;i<a;i++)
	{
		if(num[i]>max)
			max = num[i];
	}
	int secmax = num[0];
	for(int i = 1;i<a;i++)
	{
		if(num[i]!=max && num[i]>secmax)
		{
			secmax = num[i];
		}
	}
	printf("%d",secmax);
}
