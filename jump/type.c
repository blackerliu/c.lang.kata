#include <stdio.h>

int main(void)
{
	char c;
	int  i;
	long int li;
	float f;
	double d;
	long double ld;
	void *v;

	printf("C base data type size\n");
	printf("char :%d\n", sizeof(c));
	printf("int :%d\n", sizeof(i));
	printf("long int :%d\n", sizeof(li));
	printf("float :%d\n", sizeof(f));
	printf("double:%d\n", sizeof(d));
	printf("long double:%d\n", sizeof(ld));
	printf("pointer:%d\n", sizeof(v));

}
