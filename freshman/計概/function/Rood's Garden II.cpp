#include<stdio.h>
int map_range[100][100];
int map_bool[100][100];
int a,b,count;
long long n;
void water(int row,int col,int range)
{
	if(range==0) return;
	range/=2;
	int left = col-range<0?0:col-range;
	int right = col+range>b-1?b-1:col+range;
	int up = row-range<0?0:row-range;
	int down = row+range>a-1?a-1:row+range;
	for(int i = up;i<=down;i++)
	{
		for(int j = left;j<=right;j++)
		{
			if(map_bool[i][j]==0)
			{
				count--;
				map_bool[i][j]=1;
			}
		}
	}
}

int main()
{
	int range,row,col;
	scanf("%d %d %lld",&a,&b,&n);
	count = a*b;
	for(long long i = 0;i<n;i++)
	{
		scanf("%d %d %d",&range,&row,&col);
		if(range>map_range[row][col])
		{
			map_range[row][col] = range;
		}
	}
	for(int i = 0;i<a;i++)
	{
		for(int j = 0;j<b;j++)
		{
			water(i,j,map_range[i][j]);
		}
	}
	printf("%d",count);
}
