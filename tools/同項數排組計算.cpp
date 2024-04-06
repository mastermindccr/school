#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<windows.h>
#include<map>

using namespace std;
int sum;
int result;

void put(map<int,int>&vec,int num)
{
	for(int i = 2;num>1;i++)
	{
		while(num%i==0)
		{
			num/=i;
			if(!vec.count(i))
			{
				vec.insert({i,1});
			}
			else
			{
				vec[i]++;
			}
		}
	}
}

int permutation(int a,int b)
{
	map<int,int> numerator;//分子
	map<int,int> denominator;//分母 
	int c = a-b;
	if(c>b)
	{
		int tmp = c;
		c = b;
		b = tmp;
	}
	int val = 1;
	for(int i = 0;i<c;i++)
	{
		put(numerator,b+i+1);
		put(denominator,i+1);
	}
	for(auto &i : denominator)
	{
		numerator[i.first]-=i.second;
	}
	for(auto &i : numerator)
	{
		while(i.second--)
		{
			val*=i.first;
		}
	}
	return val;
}
int main()
{
	char x[30],y[30],z[30];
	cout<<"C x 取 y：";
	while(cin>>x>>y>>z)
	{
		for(int i = stoi(y);i<=stoi(z);i++)
		{
			result = permutation(stoi(x),i);
			sum+=result;
			cout<<result;
			if(i!=stoi(z))
				cout<<" + ";
			else
				cout<<" = ";
		}
		cout<<sum;
		sum = 0;
		cout<<"\nC x 取 y：";
	}
}
