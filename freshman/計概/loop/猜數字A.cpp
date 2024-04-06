#include<stdio.h>

int main()
{
	int a,b,c,d;
	scanf("%d %d\n%d %d",&a,&b,&c,&d);
	if(a==c && b==d) printf("2A0B");
	else if(a==c || b==d) printf("1A0B");
	else if(a==d && b==c) printf("0A2B");
	else if(a==d || b==c) printf("0A1B");
	else printf("0A0B");
}
