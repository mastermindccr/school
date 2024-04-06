#include<stdio.h>
#include<stdlib.h>

int main()
{
	int num,how_many,question,x,y;
	scanf("%d",&num);
	int **array = malloc(num*sizeof(int*));
	for(int i = 0;i<num;i++)
	{
		scanf("%d",&how_many);
		array[i] = malloc(how_many*sizeof(int));
		for(int j = 0;j<how_many;j++)
		{
			scanf("%d",&array[i][j]);
		}
	}
	scanf("%d",&question);
	while(question--)
	{
		scanf("%d %d",&x,&y);
		printf("%d\n",array[x][y]);
	}
	for(int i = 0;i<num;i++)
	{
		free(array[i]);
	}
	free(array);
}
