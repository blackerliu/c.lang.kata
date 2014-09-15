#include <stdio.h>

typedef struct test_struct_mem{
	int a;
	int ptr1[0];
	int b;
	int ptr2[0];
}test_struct_mem;

int main(void)
{
	char buf[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x50, 0xaa, 0x51, 0xbb, 0x52, 0xcc,0x53, 0xdd};
	test_struct_mem  *test;

	test = (test_struct_mem *)buf;

	printf("test_struct_mem->a = 0x%x\n", test->a);
	printf("test_struct_mem->b = 0x%x\n", test->b);
	printf("test_struct_mem->ptr1 = 0x%x\n", test->ptr1);
	printf("test_struct_mem->ptr1 = 0x%x\n", *test->ptr1);
	printf("test_struct_mem->ptr2 = 0x%x\n", test->ptr2);
	printf("test_struct_mem->ptr2 = 0x%x\n", *test->ptr2);
}

