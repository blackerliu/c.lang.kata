#include <stdio.h>

int fun1(int i)
{
	printf("in file [%s] func  [%s] : %d\n",__FILE__,__FUNCTION__,i);
	fun2(i);
	return 0;
}
