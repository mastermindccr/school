#include<stdio.h>
int map_range[100][100][3];
int map_count[100][100];
int r,c,count;
void swap(int *a,int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void water(int row,int col,int range)
{
	if(range==0) return;
	range/=2;
	int left = col-range<0?0:col-range;
	int right = col+range>c-1?c-1:col+range;
	int up = row-range<0?0:row-range;
	int down = row+range>r-1?r-1:row+range;
	for(int i = up;i<=down;i++)
	{
		for(int j = left;j<=right;j++)
		{
			map_count[i][j]++;
		}
	}
}
int main()
{
	int num;
	int x,y,range;
	scanf("%d %d %d",&r,&c,&num);
	for(int i = 0;i<num;i++)
	{
		scanf("%d %d %d",&range,&x,&y);
		for(int j = 0;j<3;j++)
		{
			if(!map_range[x][y][j])
			{
				map_range[x][y][j] = range;
				break;
			}
			if(j==3)
			{
				for(int a = 0;a<2;a++)
				{
					for(int b = 0;b<2;b++)
					{
						if(map_range[x][y][b]>map_range[x][y][b+1]) swap(&map_range[x][y][b],&map_range[x][y][b+1]);
					}
				}
				if(range>map_range[x][y][0])
				{
					map_range[x][y][0] = range;
				}
			}
		}
	}
	for(int i = 0;i<r;i++)
	{
		for(int j = 0;j<c;j++)
		{
			for(int k = 0;k<3;k++)
			{
				water(i,j,map_range[i][j][k]);
			}
		}
	}
	for(int i = 0;i<r;i++)
	{
		for(int j = 0;j<c;j++)
		{
			if(map_count[i][j]==1||map_count[i][j]==2)
			{
				count++;
			}
		}
	}
	printf("%d",count);
}
