int new_read_chip_id(void)
{
	typedef int (*read_id)();
 
	static U8  uNum = 5 ;
	volatile U16 *pins  = NULL ;
    volatile U16 *pfunc = NULL ;
    int nrelut = 0;
	int i;
 
// get Value must be arranged from [5, 14]
    if(uNum >=15 )
	{
		uNum = 5;
	}
	else
	{
		uNum++ ;
	}
 
    pins  = (U16*)malloc(16);
	if(pins == NULL )
	{
		return 0;
	}

    memset(pins, 16, 0);
    pins[0] = (0x1000 | 0x00C0 | uNum );// mfcr rx,gsr
	pins[1] = (0x1200 | (uNum << 4) | 0x3 );   // mov  r3,rx
	pins[2] = (0x8000 | (uNum << 8) );  // ld rx,(r0,0)
    pins[3] = (0x9000 | (uNum << 8) );  // st rx,(r0,0)
       
    pfunc = malloc(128);
	if (pins == NULL )
	{
		return 0;
	}

	memset(pfunc, 128, 0);
    i = 0;
   
    pfunc[i++] = 0x2470  ; //subi r0, 8
    pfunc[i++] = pins[3] ;                   //st rx,(r0,0)
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = 0x2470  ; // subi r0, 8
    pfunc[i++] = 0x9300  ; // st r3,(r0,0)
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = 0x2470  ; // subi r0, 8
   
    pfunc[i++] = pins[0] ;
    pfunc[i++] = pins[1] ;
    pfunc[i++] = pins[3] ;
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = 0x1211  ; // mov r1, r1
    pfunc[i++] = pins[2] ;
    pfunc[i++] = 0x8200  ; // ld r2, (r0, 0)
 
	pfunc[i++] = 0x2070  ; // addi r0, 8
	pfunc[i++] = 0x8300  ; // ld r3
	pfunc[i++] = 0x2070  ; // addi r0, 8
    pfunc[i++] = pins[2] ; // ld rx  
    pfunc[i++] = 0x2070  ; // addi r0, 8
   
    pfunc[i++] = 0x00cf  ; // jmp r15
 
    nrelut = (*((read_id)pfunc))();
 
    memset(pins, 16, 0);
    memset(pfunc, 128, 0);
 
    free(pins);
    free(pfunc);
 
	pins = NULL;
    pfunc = NULL;
   
    return nrelut;
}
