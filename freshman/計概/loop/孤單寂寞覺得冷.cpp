#include<stdio.h>

int main()
{
	int a;
	scanf("%d",&a);
	int b[a];
	for(int i = 0;i<a;i++)
	{
		scanf("%d",&b[i]);
	}
	for(int i = 0;i<a;i++)
	{
		int ans = 1;
		if(b[i]==1) ans = 0;
		else 
		{
			for(int j=2;j<b[i];j++)
			{
				if(b[i]%j==0)
				{
					ans = 0;
					break;
				}
			}
		}
		if(ans) printf("YES");
		else printf("NO");
		if(i!=a-1)
		{
			printf("\n");
		}
	}
} 
