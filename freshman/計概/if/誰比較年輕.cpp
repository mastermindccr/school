#include<stdio.h>

int main()
{
	int a,b,c,d,e,f;
	scanf("(%d,%d,%d) (%d,%d,%d)",&a,&b,&c,&d,&e,&f);
	if(a>d||(a==d && b>e) || (a==d && b==e && c>f))
		printf("Bob");
	else
		printf("Alice");
} 
