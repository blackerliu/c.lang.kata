#ifndef __COMMAND_RESOLVE_H_
#define __COMMAND_RESOLVE_H_

#define UART_OUTPUT_BUF_SIZE                            (0xFF)

#define ZEROPAD 1               /* pad with zero */
#define PLUS    4               /* show plus */
#define SPACE   8               /* space if plus */
#define LEFT    16              /* left justified */
#define SPECIAL 32              /* 0x */
#define LARGE   64              /* use 'ABCDEF' instead of 'abcdef' */
#define SIGN    2               /* unsigned/signed long */

#if 0
#define ck_do_div(n,base) ({ \
long __res; \
__res = ((unsigned long) n) % (unsigned) base; \
n = ((unsigned long) n) / (unsigned) base; \
__res; })
#else
#define ck_do_div(n,base) ({ \
long __res; \
if(base==10)				\
{							\
	__res = ((unsigned long) n) % 10; \
	n = ((unsigned long) n) / 10; \
}							\
else	if(base==8)			\
{							\
	__res = ((unsigned long) n) % 8; \
	n = ((unsigned long) n) / 8; \
}									\
else	if(base==16)			\
{								\
	__res = ((unsigned long) n) % 16; \
	n = ((unsigned long) n) / 16; \
}									\
__res; })
#endif

#define REG_GET_BIT(reg,bit)            (((reg)  >>  (bit)) & 0x01)
#define REG_SET_VAL(reg,val)           	((reg)  =  (val))

#define REG_CLR_BIT(reg,bit)                                                    \
do                                                                              \
{                                                                               \
    unsigned int Reg = (reg);                                                          \
    ((Reg)   &=  ~(1UL << (bit)));                                              \
    (reg) = Reg;                                                                \
}while(0)


#define REG_SET_BIT(reg,bit)                                                    \
do                                                                              \
{                                                                               \
    unsigned int Reg = (reg);                                                          \
    ((Reg)   |=  (1UL << (bit)));                                               \
    (reg) = Reg;                                                                \
}while(0)

typedef unsigned int CK_NATIVE_INT;
#define  CK_AUPBND         (sizeof (CK_NATIVE_INT) - 1)
#define ck_bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))
#define ck_va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (ck_bnd (A,CK_AUPBND))))
#define ck_isdigit(c) (c >= '0' && c <= '9')
#define  CK_ADNBND         (sizeof (CK_NATIVE_INT) - 1)
#define ck_va_arg(ap, T)   (*(T *)(((ap) += (ck_bnd (T, CK_AUPBND))) - (ck_bnd (T,CK_ADNBND))))
#define ck_va_end(ap)      (void) 0

#define UART_IS_RX_END()                                                 \
        ((REG_GET_BIT(rUART_SCISR,buart_scisr_rdrf)) == 0)

#define UART_IS_TX_EMPTY()                                               \
        ((REG_GET_BIT(rUART_SCISR,buart_scisr_tdre)) == 0)

#define CONFIG_SYS_PROMPT	"=>"	        /* Monitor Command Prompt	*/
#define CMD_FLAG_REPEAT		0x0001			/* repeat last command		*/
#define CONFIG_SYS_CBSIZE			256
#define CTL_BACKSPACE		('\b')

#define CONFIG_CMDLINE_EDITING
#define MAX_CMDBUF_SIZE		256
#define	GD_FLG_RELOC	0x00001		/* Code was relocated to RAM		*/
#define CONFIG_SHOW_ACTIVITY
//#define CONFIG_AUTO_COMPLETE
#define CREAD_HIST_CHAR		('!')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

#define CONFIG_SYS_MAXARGS		16
#define CONFIG_CMD_BOOTD



#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

enum
{
	chip_error_no_error = 0,
	chip_error_cmd_error
};

#endif

