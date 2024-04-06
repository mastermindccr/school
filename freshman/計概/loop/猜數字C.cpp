#include<stdio.h>

int main()
{
	int a[9];
	int b[9];
	int A=0,B=0;
	char s;
	for(int i = 0;i<9;i++)
	{
		scanf("%d%c",&a[i],&s);
	}
	for(int i = 0;i<9;i++)
	{
		scanf("%d%c",&b[i],&s);
	}
	for(int i = 0;i<9;i++)
	{
		if(a[i]==b[i])
		{
			A++;
		}
		else
		{
			for(int j = 0;j<9;j++)
			{
				if(a[i]==b[j]&&j!=i)
				{
					B++;
				}
			}
		}
	}
	printf("%dA%dB",A,B);
}
