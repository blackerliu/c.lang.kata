#include "stdio.h" 

int f(int a) 

{ 

    static c=2; 

    c=c+a*a; 

    return c; 

} 

int g(int x) 

{ 

    int c=2; 

    c=c*x; 

    return c; 

} 

void main() 

{ 

    int a,b; 

    a=f(g(3)); 

    b=g(f(3)); 

    printf("%d,%d\n",a,b); 

} 
