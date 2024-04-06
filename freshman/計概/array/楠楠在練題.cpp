#include<stdio.h>

int main()
{
	int ac,num;
	scanf("%d %d",&ac,&num);
	int array[120];
	int find;
	int ans;
	for(int i = 0;i<ac;i++)
	{
		scanf("%d",&array[i]);
	}
	for(int i = 0;i<num;i++)
	{
		ans = 0;
		scanf("%d",&find);
		for(int j = 0;array[j]!='\0';j++)
		{
			if(find==array[j])
			{
				ans = 1;
				break;
			}
		}
		if(ans) printf("YES\n");
		else printf("NO\n");
	}
} 
