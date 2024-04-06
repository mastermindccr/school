#include<stdio.h>
int map[1024][1024];
int count = 1;
int exp(int a,int b)
{
	int val=1;
	while(b--) val*=a;
	return val;
}

void put(int lux,int luy,int line);

int main()
{
	int a,b,ans=0,lux=0,luy=0,trs[2];
	scanf("%d %d",&a,&b);
	int line = exp(2,a);
	put(lux,luy,line);
	put(lux,luy+line,line);
	put(lux+line,luy,line);
	put(lux+line,luy+line,line);
	for(int i = 0;i<line;i++)
	{
		for(int j = 0;j<line;j++)
		{
			printf("%d ",map[i][j]);
			if(ans==0)
				if(map[i][j]==b)
				{
					trs[0] = i+1;
					trs[1] = j+1;
					ans==1;
				}
		}
		printf("\n");
	}
	printf("%d %d",trs[0],trs[1]);
}

void put(int lux,int luy,int line)
{
	if(line!=2)
	{
		line/=2;
		put(lux,luy,line);
		put(lux,luy+line,line);
		put(lux+line,luy,line);
		put(lux+line,luy+line,line);
	}
	else
	{
		map[lux][luy]=count++;
		map[lux][luy+1]=count++;
		map[lux+1][luy]=count++;
		map[lux+1][luy+1]=count++;
	}
}
