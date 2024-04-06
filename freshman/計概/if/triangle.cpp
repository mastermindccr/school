#include<stdio.h>

int main()
{
	int a,b,c;
	scanf("%d %d %d",&a,&b,&c);
	if(a>=b&&a>=c)
	{
		if(c+b<=a) printf("OOPS");
		else if(b*b+c*c==a*a) printf("right triangle");
		else if(b*b+c*c<a*a) printf("obtuse triangle");
		else printf("acute triangle");
	}
	else if(b>=a&&b>=c)
	{
		if(a+c<=b) printf("OOPS");
		else if(a*a+c*c==b*b) printf("right triangle");
		else if(a*a+c*c<b*b) printf("obtuse triangle");
		else printf("acute triangle");
	}
	else
	{
		if(a+b<=c) printf("OOPS");
		else if(b*b+a*a==c*c) printf("right triangle");
		else if(b*b+a*a<c*c) printf("obtuse triangle");
		else printf("acute triangle");
	}
}
