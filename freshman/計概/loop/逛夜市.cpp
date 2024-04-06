#include<stdio.h>
int main()
{
	int a;
	int money = 100;
	int stall = 0;
	char s;
	while(scanf("%d%c",&a,&s)!=EOF)
	{
		if(s==' ')
		{
			if(money-a>=0)
			{
				money-=a;
				stall++;
			}
			else break;
		}
		else 
		{
			if(money-a>=0)
			{
				stall++;
			}
			break;
		}
	}
	printf("%d",stall);
}
