#include<stdio.h>

int leap_year(int a)
{
	if(a%400==0)return 366;
	else if(a%100==0)return 365;
	else if(a%4==0)return 366;
	else return 365;
}
int main()
{
	int year,month,date;
	int mon1[]={31,28,31,30,31,30,31,31,30,31,30,31};
	int mon2[]={31,29,31,30,31,30,31,31,30,31,30,31};
	scanf("%d %d %d",&year,&month,&date);
	int day = 0;
	if(year>2020)
	{
		for(int i = 2020;i<year;i++)
			day+=leap_year(i);
		if(leap_year(year)==365)
			for(int i = 0;i<month-1;i++)
				day+=mon1[i];	
		else
			for(int i = 0;i<month-1;i++)
				day+=mon2[i];
		day+=date-1;
		printf("%d",(3+day)%7);
	}
	else if(year<2020)
	{
		for(int i = 2019;year<i;i--)
			day+=leap_year(i);
		if(leap_year(year)==365)
		{
			for(int i = 11;month<i+1;i--)
				day+=mon1[i];
			day+=mon1[month-1]+1-date;
		}
		else
		{
			for(int i = 11;month<i+1;i--)
				day+=mon2[i];
			day+=mon2[month-1]+1-date;
		}
		printf("%d",(10-day%7)%7);
	}
	else
	{
		for(int i = 0;i<month-1;i++)
				day+=mon2[i];
		day+=date-1;
		printf("%d",(day+3)%7);
	}
}
