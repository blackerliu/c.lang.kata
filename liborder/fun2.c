#include <stdio.h>

int fun2(int i)
{
	printf("in file [%s] func [%s]  : %d\n",__FILE__,__FUNCTION__,i);
	return 0;
}

int fun4(int i)
{
	printf("in file [%s] func [%s] : %d\n",__FILE__,__FUNCTION__,i);
	return 0;
}
