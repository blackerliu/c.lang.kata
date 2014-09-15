#define NULL ((void*)0)

int new_vip_chip_id_read(void)
{
 	typedef unsigned char U8;
	typedef unsigned short U16;
	typedef int (*read_id)();

	static U8  reg_idx = 2 ;
	volatile U16 *pins  = NULL ;
    volatile U16 *pfunc = NULL ;
    int nresult = 0;
	int i = 0;
 
	// get Value must be arranged from [2, 7]
    if(reg_idx >=7)
	{
		reg_idx = 2;
	}
	else
	{
		reg_idx++ ;
	}
 
    pins  = (U16*)malloc(16);
	if(pins == NULL )
	{
		return 0;
	}

    memset(pins, 16, 0);
	i = 0;

	pins[0] = (0x24f0);  					// subi 	r0, r0, 16
	pins[1] = (0x20f0);  					// addi    r0, r0, 16
	pins[2] = (0x8008 | (reg_idx << 8));  	// ld rx,(r8,0)
    pins[3] = (0x9008 | (reg_idx << 8));  	// st rx,(r8,0)
	pins[4] = (0x1211);  					// mov  r1,r1

    pfunc = (U16*)malloc(128);
	if (pfunc == NULL )
	{
		if(pins != NULL)
		{
			free(pins);
			pins = NULL;
		}
		return 0;
	}

	memset(pfunc, 128, 0);
    i = 0;

//	printf("dync_read_id malloc addr:0x%08X\n", (unsigned int)pfunc);
	pfunc[i++] = 0x2470;        //subi    r0, r0, 8
	pfunc[i++] = 0x9f00;        //st  r15, (r0, 0)
	pfunc[i++] = pins[0];        //subi    r0, r0, 16
	pfunc[i++] = 0x9820;        //st  r8, (r0, 8)
	pfunc[i++] = 0x1208;        //mov r8, r0
	pfunc[i++] = pins[4];
	pfunc[i++] = 0x6000|(reg_idx);        //movi    r7, 0
	pfunc[i++] = 0x9018|(reg_idx<<8);        //st  r7, (r8, 4)
	pfunc[i++] = 0x6000|(reg_idx);        //movi    r7, 0
	pfunc[i++] = pins[3];        //st  r7, (r8, 0)                                                                          
	pfunc[i++] = 0x33d0|(reg_idx);        //bgeni   r7, 29
	pfunc[i++] = 0x35f0|(reg_idx);        //bseti   r7, r7, 31
	pfunc[i++] = pins[3];        //st  r7, (r8, 0)
	pfunc[i++] = pins[2];        //ld  r7, (r8, 0)
	pfunc[i++] = 0x3540|(reg_idx);        //bseti   r7, r7, 20
	pfunc[i++] = 0x3550|(reg_idx);        //bseti   r7, r7, 21
	pfunc[i++] = pins[3];        //st  r7, (r8, 0)
	pfunc[i++] = pins[2];        //ld  r7, (r8, 0)
	pfunc[i++] = 0x34d0|(reg_idx);        //bseti   r7, r7, 13
	pfunc[i++] = 0x34f0|(reg_idx);        //bseti   r7, r7, 15
	pfunc[i++] = pins[3];        //st  r7, (r8, 0)
	pfunc[i++] = pins[2];        //ld  r7, (r8, 0)
	pfunc[i++] = 0x3470|(reg_idx);        //bseti   r7, r7, 7
	pfunc[i++] = 0x3480|(reg_idx);        //bseti   r7, r7, 8
	pfunc[i++] = pins[3];        //st  r7, (r8, 0)
	pfunc[i++] = pins[2];        //ld  r7, (r8, 0)
	pfunc[i++] = 0x8000|(reg_idx<<8)|(reg_idx);        //ld  r7, (r7, 0)
	pfunc[i++] = 0x3f00|(reg_idx);        //lsri    r7, r7, 16
	pfunc[i++] = 0x2e37|(reg_idx);      	//andi	r7, r7, 3
 	pfunc[i++] = 0x9018|(reg_idx<<8);      	//st	r7, (r8, 4)
	pfunc[i++] = 0x8018|(reg_idx<<8);      	//ld	r7, (r8, 4)
	pfunc[i++] = 0x1202|(reg_idx<<4);      	//mov	r2, r7
	pfunc[i++] = pins[4];
	pfunc[i++] = 0x1280;      	//mov	r0, r8
	pfunc[i++] = 0x8820;      	//ld	r8, (r0, 8)
	pfunc[i++] = pins[1];      	//addi	r0, r0, 16
	pfunc[i++] = 0x8f00;      	//ld	r15, (r0,0)
	pfunc[i++] = 0x2070;      	//addi	r0, r0, 8
	pfunc[i++] = 0x00cf;      	//jmp	r15
	pfunc[i++] = 0xf7ff;
	pfunc[i++] = 0xf7ff; 

	// flush cache
	__asm__ __volatile__(								
		   "   movi    r3, 0x23;\n"
		   "   movi    r4, 0x13;\n"
		   "   idly4;\n"
		   "   sync;\n"
		   "   mtcr    r3, cr17;\n"
		   "   mtcr    r4, cr17;\n"
		   :
		   :
		   :"r3","r4"
		   );
	  

    nresult = (*((read_id)pfunc))();
  //  printf("dync_read_id direct read:0x%08X\n", (*(volatile unsigned int*)0xa030a180));
    memset(pins, 16, 0);
    memset(pfunc, 128, 0);
 
    free(pins);
    free(pfunc);
 
	pins = NULL;
    pfunc = NULL;
   
    return nresult;
}

