#include "command.h"
#include "stddef.h"

unsigned int do_chip_set_reg(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int addr;
	unsigned int regval;
	
	if (argc < 3) {
		return 1;
	}

	addr = (unsigned int)strtoul(argv[1], NULL, 10);
	regval = (unsigned int)strtoul(argv[2], NULL, 10);
	

	if((addr == 0)||(addr%4 != 0))
	{
		return 2;
	}

	log_printf("[Command] will set value into reg.\n");
	//*(volatile  unsigned int *)addr = regval;
	return 0;
}

unsigned int do_chip_get_reg(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int addr;
	unsigned int regval = 5;

	if (argc < 1) {
		return 1;
	}

	addr = (unsigned int)strtoul(argv[1], NULL, 10);
	
	if((addr == 0)||(addr%4 != 0))
	{
		return -1;
	}
	else
	{
		log_printf("[Command] will get value from reg.\n");
		regval = 0x55AA55BB;
//		regval = *(volatile unsigned int *)addr;
		return regval;
	}
}



cmd_tbl_t cmd_set_reg={
	.name = "set",
	.maxargs = 3,
	.repeatable = 1,
	.cmd = do_chip_set_reg,
	.usage = "set the vallue of registers"
};


cmd_tbl_t cmd_get_reg={
	.name = "get",	
	.maxargs = 2,
	.repeatable = 1,
	.cmd = do_chip_get_reg,
	.usage = "get the vallue of registers"
};



