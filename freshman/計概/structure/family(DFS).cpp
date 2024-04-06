#include<stdio.h>
#include<string.h>
int r;
int young[35][35];//former one is younger
int old[35][35];//former one is older
int age[35];
char name[35][35];
int number(char* person)
{
	for(int i = 0;i<r;i++)
	{
		if(!strcmp(person,name[i])) return i;
	}
	age[r] = 1000;
	strcpy(name[r],person);
	return r++;
}
void dfs(int person,int years)
{
	age[person] = years;
	for(int i = 0;i<r;i++)
	{
		if(age[i]==1000)
		{
			if(old[i][person]) dfs(i,years+1);
			if(young[i][person]) dfs(i,years-1);
		}
	}
}
int main()
{
	int num;
	char p1[35],p2[35];
	scanf("%d",&num);
	while(num--)
	{
		scanf("%s %s",p1,p2);
		int s = number(p1),t = number(p2);
		young[s][t] = 1;
		old[t][s] = 1; 
	}
	int cur = number("Dong");
	dfs(cur,0);
	for(int i = 0;i<r;i++)
	{
		if(i==cur) continue;
		printf("%s ",name[i]);
		if(age[i]==1000) printf("unknown\n");
		else if(age[i] > 0) printf("elder\n");
		else if(age[i] == 0) printf("same\n");
		else printf("younger\n");
	}
}
