#include<stdio.h>
#include<string.h>
int main()
{
	int num,a,b;
	char w[15];
	scanf("%d",&num);
	char str[15];
	while(num--)
	{
		scanf("%d %d %s",&a,&b,w);
		while(scanf("%s",str))
		{
			if(!strcmp(str,"end")) break;
			if(strcmp(str,w)<0)
			{
				for(int i = 0;str[i]!='\0';i++)
				{
					if((int)str[i] + a > 122) str[i]=(char)(int)str[i] - 26;
						printf("%c",(char)(int)str[i] + a);
				}
			}
			else if(strcmp(str,w)>0)
			{
				for(int i = 0;str[i]!='\0';i++)
				{
					if((int)str[i] - b < 97)
					{
						printf("%c",(char)(int)str[i] - b + 26);
					}
					else
						printf("%c",(char)(int)str[i] - b);
				} 
			}
			else
				printf("%s",w);
			printf(" ");
		}
		puts("");
	}
}
