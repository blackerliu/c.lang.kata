int  foo( int x , int  n)
{
    int val;
    val =1;
    
    if (n>0) 
    {
        if (n%2 == 1)  val = val *x;
        val = val * foo(x*x , n/2);
    }
    return val;
}

void main()
{
	printf("result:%d\n", foo(3,4));
}
