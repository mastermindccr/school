#include<stdio.h>

int main()
{
	int a;
	int sum = 0;
	int max = 0;
	int num = 0;
	char s;
	while(scanf("%d%c",&a,&s)!=EOF)
	{
		num++;
		sum+=a;
		if(a>max) max = a;
		if(s != ' ') break;
	}
	printf("%d %d %d",num,sum,max);
}
