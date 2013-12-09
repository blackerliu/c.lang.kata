#include <stdio.h>
#include "define.h"
int sum = 0;

int external_add(int a, int b)
{
	return (a+b);
} 

int external_sub(int a, int b)
{
	return (a-b);
}

int external_customer(int a,int b)
{
	return (a*10+b/10+a*b+a|b);
}
