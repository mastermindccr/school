#include<stdio.h>

int main()
{
	int a;
	float area = 0;
	scanf("%d",&a);
	int b[2][1000];
	for(int i = 0;i<a;i++)
	{
		scanf("%d %d",&b[0][i],&b[1][i]);
	}
	b[0][a]=b[0][0];
	b[1][a]=b[1][0];
	for(int i = 0;i<a;i++)
	{
		area+=b[0][i]*b[1][i+1]-b[1][i]*b[0][i+1];
	}
	if(area<0) area = -area;
	printf("%.1f",area/2);
} 
