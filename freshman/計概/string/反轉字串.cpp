#include<stdio.h>

int main()
{
	int num;
	int string;
	scanf("%d",&num);
	while(num--)
	{
		scanf("%d",&string);
		char str[string][110];
		for(int i = 0;i<string;i++)
		{
			scanf("%s",str[i]);
		}
		while(string--)
		{
			printf("%s ",str[string]);
		}
		puts("");
	}
}
