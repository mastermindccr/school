#include<stdio.h>
const int arr[10] = {1,2,4,8,16,32,60,120,240,480};
int have[10];
int a,sum;

int valid()
{
	int count = 0;
	for(int i = 0;i<6;i++)
	{
		if(have[i]) count+= arr[i];
		if(count>=60) return 0;
	}
	count = 0;
	for(int i = 6;i<10;i++)
	{
		if(have[i]) count+=arr[i];
		if(count>=720) return 0;
	}
	return 1;
}

void time(int a,int pos)
{
	if(a==0)
	{
		if(valid())
			for(int i = 0;i<10;i++)
			{
				if(have[i]) sum+=arr[i];
			}
		return;
	}
	for(int i = pos;i<10;i++)
	{
		if(!have[i])
		{
			have[i] = 1;
			time(a-1,i);
			have[i] = 0;
		}
	}
}

int main()
{
	int num;
	scanf("%d",&num);
	while(num--)
	{
		sum = 0;
		scanf("%d",&a);
		time(a,0);
		printf("%d\n",sum);
	}	
}
