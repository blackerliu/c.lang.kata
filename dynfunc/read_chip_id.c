int vip_chip_id_read (void) //
{
    volatile unsigned int chipid = 0;
    unsigned int reg_addr = 0;

	// reg: 0xA030A180
	reg_addr = (0xA0 << 24);
	reg_addr = reg_addr | (0x30 << 16);
   	reg_addr = reg_addr | (0xA0 << 8) ;
	reg_addr = reg_addr | 0x180 ;

    chipid = (*(volatile unsigned int *)(reg_addr) >> 16) & 3;

    return chipid;
}

int test(void)
{
	return vip_chip_id_read();
}
