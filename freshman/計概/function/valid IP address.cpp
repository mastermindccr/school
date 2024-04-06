#include<stdio.h>
#include<string.h>
#include<math.h>

int main()
{
	int num;
	scanf("%d",&num);
	int IP;
	char address[100];
	while(num--)
	{
		int ans = 1;
		scanf("%d %s",&IP,address);
		if(IP==4)
		{
			int dot = 0;
			int pos = 0;
			char temp[100];
			double s = 0;
			for(int r = 0;temp[r];r++)
			{
				temp[r] = '\0';
			}
			for(int i = 0;address[i];i++)
			{
				if(address[i]>=48&&address[i]<=57)
				{
					temp[pos] = address[i];
					pos++;
				}
				else if(address[i]=='.')
				{
					for(int j = 0;temp[j];j++)
					{
						s+=pow(10,strlen(temp)-j-1)*((double)temp[j]-48);
					}
					if(pos>3 || pos<1 || (pos!=1 && temp[0]=='0') || s>255 || s<0)
					{
						ans = 0;
						break;
					}
					pos = 0;
					dot++;
					s=0;
					for(int r = 0;temp[r];r++)
					{
						temp[r] = '\0';
					}
				}
				else {
					ans = 0;
					break;
				}
			}
			if(pos>3 || pos<1 || (pos!=1 && temp[0]=='0') || s>255 || s<0 || dot!=3)
			{
				ans = 0;
			}
		}
		if(IP==6)
		{
			int pos = 0;
			int com = 0;
			for(int i = 0;address[i];i++)
			{
				if(address[i]!=':')
				{
					if (!(((int)address[i]>=48 && (int)address[i]<=57)||((int)address[i]>=65 && (int)address[i]<=70)||((int)address[i]>=97 && (int)address[i]<=102)))
					{
						ans = 0;
						break;
					}
					else
					{
						pos++;
					}
				}
				else
				{
					if(pos>4 || pos<1)
					{
						ans = 0;
						break;
					}
					com++;
					pos=0;
				}
			}
			if(com!=7 || pos>4 || pos<1) ans = 0;
		}
		if(ans) printf("Valid IP\n");
		else printf("QAQ\n");
		for(int i = 0;address[i];i++)
		{
			address[i]='\0';
		}
	}
}
