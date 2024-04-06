#include<stdio.h>

void move(int disk,char a,char b,char c);

int main()
{
	int a;
	int b[15];
	for(a = 0;scanf("%d",&b[a])!=EOF;a++);
	for(int i = 0;i<a;i++)
	{
		move(b[i],'A','B','C');
		printf("finish\n");
	}
} 

void move(int disk,char a,char b,char c)
{
	if(disk==1) printf("Move ring %d from %c to %c\n",disk,a,c);
	if(disk!=1)
	{
		move(disk-1,a,c,b);
		printf("Move ring %d from %c to %c\n",disk,a,c);
		move(disk-1,b,a,c);
	}
}
