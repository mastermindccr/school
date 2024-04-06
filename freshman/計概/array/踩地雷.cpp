#include<stdio.h>

int main()
{
	int a,b;
	scanf("%d %d",&a,&b);
	int arr[a+2][b+2] = {0};
	for(int i = 1;i<=a;i++)
	{
		for(int j = 1;j<=b;j++)
		{
			scanf("%d",&arr[i][j]);
		}
	}
	for(int i = 1;i<=a;i++)
	{
		for(int j = 1;j<=b;j++)
		{
			if(arr[i][j]==1) printf("-1 ");
			else
			{
				int pt = 0;
				if(arr[i-1][j-1]==1) pt++;
				if(arr[i-1][j]==1) pt++;
				if(arr[i-1][j+1]==1) pt++;
				if(arr[i][j-1]==1) pt++;
				if(arr[i][j+1]==1) pt++;
				if(arr[i+1][j-1]==1) pt++;
				if(arr[i+1][j]==1) pt++;
				if(arr[i+1][j+1]==1) pt++;
				printf("%d ",pt);
			}
		}
		printf("\n");
	}
}
