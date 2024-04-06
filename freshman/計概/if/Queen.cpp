#include<stdio.h>

int main()
{
	int a,b,c,d;
	scanf("(%d,%d) (%d,%d)",&a,&b,&c,&d);
	if((a-c)==(b-d)||(a-c)==(d-b)||a==c||b==d)
	{
		printf("D");
	}
	else {
		printf("S");
	}
}
