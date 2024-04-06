#include<stdio.h>

int convert(char ch)
{
	if(ch=='I') return 1;
	if(ch=='V') return 5;
	if(ch=='X') return 10;
	if(ch=='L') return 50;
	if(ch=='C') return 100;
	if(ch=='D') return 500;
	if(ch=='M') return 1000;
	if(ch=='\0') return 0;
}

int main()
{
	int num;
	char string[50];
	scanf("%d",&num);
	while(num--)
	{
		int sum=0;	
		scanf("%s",string);
		for(int i = 0;string[i]!='\0';i++)
		{
			if(convert(string[i+1])>convert(string[i]))
			{
				sum+=convert(string[i+1])-convert(string[i]);
				i+=1;
			}
			else
			{
				sum+=convert(string[i]);
			}
		}
		printf("%d\n",sum);
	}
}
