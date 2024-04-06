#include<stdio.h>
void reduce(int numerator, int denominator, int* reduce_numerator, int* reduce_denominator)
{
	int a = numerator;
	int b = denominator;
	while(a!=0&&b!=0)
	{
		if(a>=b) a%=b;
		else b%=a;
	}
	if(a==0)
	{
		*reduce_numerator = numerator/b;
		*reduce_denominator = denominator/b;
	}
	else
	{
		*reduce_numerator = numerator/a;// &(variable name) === (pointer variable name)  (variable name) === *(pointer vaiable name)
		*reduce_denominator = denominator/a;
	}
}

int main()
{
	int arr1;
	int arr2;
	int a[1] = {0}, b[1] = {0};//build an array so that the pointer can point it
	int *ptr1 = a;
	int *ptr2 = b;
	printf("Enter a fraction:");
	scanf("%d/%d",&arr1,&arr2);
	reduce(arr1,arr2,ptr1,ptr2);//i.e we pass an array to modify its value by pointer
	float val = float(arr1)/float(arr2);
	printf("In lowest term:%d/%d\nThe value is:%g",*ptr1,*ptr2,val);
}
