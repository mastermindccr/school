#include<stdio.h>

int main()
{
	int num;
	int k;
	char string[150];
	scanf("%d",&num);
	while(num--)
	{
		scanf("%d",&k);
		scanf("%s",&string);
		for(int i = 0;string[i]!='\0';i++)
		{
			if((int)string[i]-k>=97)
			{
				string[i] = (char)(int)string[i]-k;
			}
			else
			{
				string[i] = (char)(int)string[i]-k+26;
			}
			printf("%c",string[i]);
		}
		printf("\n");
	}
} 
