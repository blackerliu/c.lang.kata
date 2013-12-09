#include<setjmp.h>

static jmp_buf buf;

main()    
{
    volatile  int b;
    b =23;
    
    if(setjmp(buf)!=0)
    {
        printf("%d ", b); 
        exit(0);
    }
    b=17;
    longjmp(buf , 1);
    printf("%d",b);
} 
