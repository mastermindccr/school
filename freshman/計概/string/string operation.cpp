#include<stdio.h>
#include<string.h>
char cmd[100];
int main()
{
	int num;
	char string[150][150];
	scanf("%d",&num);
	for(int i = 0;i<num;i++)
	{
		scanf("%s",&string[i]);
	}
	int a,b;
	while(scanf("%s %d %d",cmd,&a,&b)!=EOF)
	{
		if(!strcmp(cmd,"add"))
		{
			strcat(string[a],string[b]);
			printf("%s\n",string[a]);
		}
		else if(!strcmp(cmd,"sub"))
		{
			for(int i = 0;string[a][i];i++)
			{
				if(string[a][i]==string[b][0])
				{
					for(int j = i;string[a][j];j++)
					{
						string[a][j] = string[a][j+1];
					}
				}
			}
			printf("%s\n",string[a]);
		}
		else if(!strcmp(cmd,"eq"))
		{
			if(!strcmp(string[a],string[b])) printf("same\n");
			else printf("different\n");
		}
		else printf("Oops\n");
	}
}
