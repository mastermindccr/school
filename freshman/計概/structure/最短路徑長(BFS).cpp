#include<stdio.h>

typedef struct{
	int link[120];
	int length;
}vertex;

int main()
{
	int v,e,start,end,length = 0,non = 1;
	scanf("%d %d",&v,&e);
	vertex arr[v];
	for(int i = 0;i<v;i++)
	{
		arr[i].length = -1;
		for(int j = 0;j<120;j++)
		{
			arr[i].link[j] = 0;
		}
	}
	while(e--)
	{
		scanf("%d %d",&start,&end);
		arr[start].link[end] = 1;
		arr[end].link[start] = 1;
	}
	scanf("%d %d",&start,&end);
	arr[start].length = 0;
	while(arr[end].length == -1)
	{
		non = 0;
		for(int i = 0;i<v;i++)
		{
			for(int j = 0;j<v;j++)
			{
				if(arr[i].length == length && arr[j].length == -1 && arr[i].link[j] == 1)
				{
					arr[j].length = length + 1;
					non = 1;
				}
			}
		}
		if(!non) break;
		length++;
	}
	if(non == 0) printf("Oops\n");
	else
	printf("%d\n",length);
}
