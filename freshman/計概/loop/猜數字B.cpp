#include<stdio.h>

int main()
{
	int a[10];
	int b[10];
	int A=0;
	char s;
	for(int i = 0;i<10;i++)
	{
		scanf("%d%c",&a[i],&s);
	}
	for(int i = 0;i<10;i++)
	{
		scanf("%d%c",&b[i],&s);
	}
	for(int i = 0;i<10;i++)
	{
		if(a[i]==b[i])
		{
			A++;
		}
	}
	printf("%dA%dB",A,10-A);
}
