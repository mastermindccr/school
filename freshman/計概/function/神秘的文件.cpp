#include<stdio.h>
int left = 1;
int bef[100000];
int aft[100000];

void scramble(int l,int num)
{
	if(num == 1) return ;
	for(int i = l;i<=num/2+l-1;i++)
	{
		aft[i+num/2] = bef[i];
		aft[i] = bef[i+num/2];
	}
	for(int i = l;i<=num+l-1;i++)
	{
		bef[i] = aft[i];
	}
	scramble(l,num/2);
	scramble(l+num/2,num/2);
}
int main()
{
	int a;
	scanf("%d",&a);
	for(int i = 1;i<=a;i++)
	{
		bef[i] = i;
		aft[i] = i;
	}
	scramble(left,a);
	for(int i = 1;i<=a;i++)
	{
		printf("%d ",bef[i]);
	}
} 
