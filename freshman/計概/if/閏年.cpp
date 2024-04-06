#include<stdio.h> 

int main()
{
	int a;
	scanf("%d",&a);
	if(a%400==0)printf("366");
	else if(a%100==0)printf("365");
	else if(a%4==0)printf("366");
	else printf("365");
}
