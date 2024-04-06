#include<stdio.h>

int main()
{
	int a,b,c,x,y;
	char s;
	int square = 0;
	scanf("%d %d\n%d",&a,&b,&c);
	int arr[a+2][b+2]={0};
	int arr2[c*2];
	for(int i = 0;i<c*2;i++)
	{
		scanf("%d%c",&arr2[i],&s);
	}
	for(int i = 0;i<c*2;i+=2)
	{
		arr[arr2[i]][arr2[i+1]] = 1;
		arr[arr2[i]][arr2[i+1]+1] = 1;
		arr[arr2[i]][arr2[i+1]+2] = 1;
		arr[arr2[i]+1][arr2[i+1]] = 1;
		arr[arr2[i]+1][arr2[i+1]+1] = 1;
		arr[arr2[i]+1][arr2[i+1]+2] = 1;
		arr[arr2[i]+2][arr2[i+1]] = 1;
		arr[arr2[i]+2][arr2[i+1]+1] = 1;
		arr[arr2[i]+2][arr2[i+1]+2] = 1;
	}
	for(int i = 1;i<=a;i++)
	{
		for(int j = 1;j<=b;j++)
		{
			if(!arr[i][j])
			{
				square++;
			}
		}
	}
	printf("%d",square);
}
