#include<stdio.h>
int a[100];
int all;
void swap(int *a,int *b)
{
	int temp = *a;
	*a=*b;
	*b=temp;
}

void bubble_sort(int a[])
{
	for(int i = 0;i<all-1;i++)
	{
		for(int j = 0;j<all-1;j++)
		{
			if(a[j]>a[j+1]) swap(&a[j],&a[j+1]);
		}
	}
}

int main()
{
	all = 0;
	while(scanf("%d",&a[all])!=EOF)
	{
		all++;
	}
	bubble_sort(a);
	for(int i = 0;i<all;i++)
	{
		printf("%d",a[i]);
	}
}
