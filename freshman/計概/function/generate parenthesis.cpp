#include<stdio.h>
int have[24];
int num;
void arrange(int pos,int sum)
{
	if(sum<0) return;
	if(pos==num*2)
	{
		if(sum==0)
		{
			for(int i = 0;i<num*2;i++)
			{
				if(have[i]==0)
				{
					printf("(");
				}
				else
				printf(")");
			}
			printf("\n");
		}
		return;
	}
		for(int i = 0;i<2;i++)
		{
			have[pos] = i;
			have[pos]==0?arrange(pos+1,sum+1):arrange(pos+1,sum-1);
		}
}

int main()
{
	scanf("%d",&num);
	arrange(0,0);
}
