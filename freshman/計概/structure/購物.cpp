#include<stdio.h>
#include<string.h>
char bub_sort[15][15];
typedef struct inf{
    int pri[15];
    int money[15];
    char item[15][15];
    int have[15];
}buy;
void swap_int(int* a,int* b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void swap_str(char* a,char* b)
{
    char temp[15] = {'\0'};
    strcpy(temp,a);
    for(int i = 0;a[i]!='\0';i++)
    {
        a[i] = '\0';
    }
    strcpy(a,b);
    for(int i = 0;b[i]!='\0';i++)
    {
        b[i] = '\0';
    }
    strcpy(b,temp);
}
void sort(buy *a,int num)
{
    for(int i = 0;i<num-1;i++)
    {
        for(int j = 0;j<num-1;j++)
        {
            if(a->pri[j+1]>a->pri[j])
            {
                swap_int(&a->pri[j+1],&a->pri[j]);
                swap_int(&a->money[j+1],&a->money[j]);
                swap_str(a->item[j+1],a->item[j]);
            }
        }
    }
}
int main()
{
    int num,money,list;
    scanf("%d",&num);
    while(num--)
    {
        buy a;
        for(int i = 0;i<15;i++)
        {
            a.have[i] = 0;
        }
        int spend = 0;
        scanf("%d %d",&money,&list);
        for(int i = 0;i<list;i++)
        {
            scanf("%d %d %s",&a.pri[i],&a.money[i],a.item[i]);
        }
        sort(&a,list);
        for(int i = 0;i<list;i++)
        {
            if(money>=a.money[i])
            {
                money-=a.money[i];
                spend+=a.money[i];
                a.have[i] = 1;
            }
        }
        printf("%d ",spend);
        int count = 0;
        for(int i = 0;i<list;i++)
        {
            if(a.have[i])
            {
                for(int j = 0;a.item[i][j]!='\0';j++)
                {
                    bub_sort[count][j] = a.item[i][j];
                }
                count++;
            }
        }
        for(int i = 0;i<count-1;i++)
        {
            for(int j = 0;j<count-1;j++)
            {
                if(strcmp(bub_sort[j],bub_sort[j+1])>0) swap_str(bub_sort[j],bub_sort[j+1]);
            }
        }
        for(int i = 0;i<count;i++)
        {
            printf("%s ",bub_sort[i]);
        }
        printf("\n");
        for(int i = 0;i<count;i++)
        {
            for(int j = 0;j<15;j++)
            {
                bub_sort[i][j] = '\0';
            }
        }
    }
}
