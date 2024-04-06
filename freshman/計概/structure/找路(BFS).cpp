#include<stdio.h> 
typedef struct{
	int val;
	int length;
}map;
int main()
{
	int row,col,length = 0;
	int s_r,s_c,e_r,e_c;
	scanf("%d %d",&row,&col);
	map arr[row][col];
	for(int i = 0;i<row;i++)
	{
		for(int j = 0;j<col;j++)
		{
			scanf("%d",&arr[i][j].val);
			arr[i][j].length = -1;
			if(arr[i][j].val == -1) arr[i][j].length = 1000;
			else if(arr[i][j].val == -2)
			{
				arr[i][j].length = 0;
				s_r = i;
				s_c = j;
			}
			else if(arr[i][j].val == -3)
			{
				e_r = i;
				e_c = j;
			}
		}
	}
	while(arr[e_r][e_c].length == -1)
	{
		int ans = 1;
		for(int i = 0;i<row;i++)
		{
			for(int j = 0;j<col;j++)
			{
				if(length == arr[i][j].length)
				{
					if(i>0 && arr[i-1][j].length == -1) arr[i-1][j].length = length + 1;
					if(i<row-1 && arr[i+1][j].length == -1) arr[i+1][j].length = length + 1;
					if(j<col-1 && arr[i][j+1].length == -1) arr[i][j+1].length = length + 1;
					if(j>0 && arr[i][j-1].length == -1) arr[i][j-1].length = length + 1;
				}
				if(arr[e_r][e_c].length != -1)
				{
					ans = 0;
					break;
				}
			}
			if(!ans) break;
		}
		length++;
	}
	printf("%d",length);
}
