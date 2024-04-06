#include<stdio.h>
int main()
{
	int a[200];
	int num = 0;
	char s;
	while(scanf("%d%c",&a[num],&s)!=EOF)
	{
		num++;
		if(s!=' ') break;
	}
	for(int i = 2;i<num-1;i++)
	{
		if(a[0]>a[1])
		{
			a[1]+=a[i];
		}
		else
		{
			a[0]+=a[i];
		}
	}
	if(a[1]>a[0]) printf("%dm%ds",(a[0]+a[num-1])/60,(a[0]+a[num-1])%60);
	else printf("%dm%ds",(a[1]+a[num-1])/60,(a[1]+a[num-1])%60);
}
