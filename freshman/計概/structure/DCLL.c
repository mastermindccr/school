#include<stdio.h>
#include<stdlib.h>
typedef struct node{
    int val;
    struct node *next;
    struct node *prev;
};
int main()
{
    int num;
    char order[10];
    scanf("%d",&num);
    struct node *TP = malloc(sizeof(struct node*));
    TP->val = 1;
    TP->next = TP;
    TP->prev = TP;
    while(num--)
    {
        scanf("%s",order);
        if(order[0] == 'W')
        {
            printf("%d\n",TP->val);
        }
        else if(order[0] == 'I')
        {
            struct node *tmp,*current = malloc(sizeof(struct node*));
            scanf("%d",&current->val);
            tmp = TP->next;
            current->next = tmp;
            TP->next = current;
            tmp->prev = current;
            current->prev = TP;
        }
        else if(order[0] == 'R')
        {
            TP = TP->next;
        }
        else
        {
            TP = TP->prev;
        }
    }
}
