#include "command_resolve.h"
#include "command.h"
#include "stddef.h"

static int ctrlc_disabled = 0;	/* see disable_ctrl() */
static int ctrlc_was_pressed = 0;


int ck_skip_atoi(const char **s)
{
        int i=0;

        while (ck_isdigit(**s))
                i = i*10 + *((*s)++) - '0';
        return i;
}

unsigned int ck_strnlen(const char * s, unsigned int count)
{
        const char *sc;

        for (sc = s; count-- && *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}


/*****************************************************************************
 * Function    : ck_number
 
 * Description : 将整数按特定的数制轮换成字符串
  
 * Arguments   : buf            字符缓冲区首地址
                 end            字符缓冲区尾地址
                 num            要转换的整数
                 base           基数
                 size           域宽
                 precision      精度
                 type           标志位
 * Returns     : 
 
 * Other       :  
 ****************************************************************************/
char *
ck_number (char *buf, char *end, long long num, int base, int size,
           int precision, int type)
{
#if 0
    char c, sign, tmp[66];
    const char *digits;
    const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
            size--;
        }
        else if (type & PLUS)
        {
            sign = '+';
            size--;
        }
        else if (type & SPACE)
        {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
    {
        while (num != 0)
       	{
			tmp[i++] = digits[ck_do_div (num, base)];
       	}
    }
	if (i > precision)
        precision = i;
    size -= precision;
    if (!(type & (ZEROPAD + LEFT)))
    {
        while (size-- > 0)
        {
            if (buf <= end)
                *buf = ' ';
            ++buf;
        }
    }
    if (sign)
    {
        if (buf <= end)
            *buf = sign;
        ++buf;
    }
    if (type & SPECIAL)
    {
        if (base == 8)
        {
            if (buf <= end)
                *buf = '0';
            ++buf;
        }
        else if (base == 16)
        {
            if (buf <= end)
                *buf = '0';
            ++buf;
            if (buf <= end)
                *buf = digits[33];
            ++buf;
        }
    }
    if (!(type & LEFT))
    {
        while (size-- > 0)
        {
            if (buf <= end)
                *buf = c;
            ++buf;
        }
    }
    while (i < precision--)
    {
        if (buf <= end)
            *buf = '0';
        ++buf;
    }
    while (i-- > 0)
    {
        if (buf <= end)
            *buf = tmp[i];
        ++buf;
    }
    while (size-- > 0)
    {
        if (buf <= end)
            *buf = ' ';
        ++buf;
    }
#endif
    return buf;
}


int had_ctrlc (void)
{
	return ctrlc_was_pressed;
}

//unsigned int gBootSystemClock = 27000000;
//
void uart_init(void)
{
#if 0
	U32 t = rUART_SCICR;
	/*GXUART_8BITS_NO_PARITY*/
	REG_CLR_BIT(rUART_SCICR,buart_scicr_fl);
    REG_CLR_BIT(rUART_SCICR,buart_scicr_pe);

	/*GXUART_STOP_1_0*/
	REG_CLR_BIT(rUART_SCICR,buart_scicr_stop);

	/*GXUART_BAUDRATE_115200*/
	REG_SET_VAL(rUART_SCIBR,gBootSystemClock/(115200 * 16));

	REG_SET_BIT(rUART_SCICR,buart_scicr_re);
	REG_SET_BIT(rUART_SCICR,buart_scicr_te);
#endif
}

#if 0

void printf(const char *fmt, ...)
{
	static signed char  buf[UART_OUTPUT_BUF_SIZE];
    signed char         *args;
    signed char         *str;
    U32         		len = 0;
	U32 i = 0 ;

	memset(buf,0,UART_OUTPUT_BUF_SIZE);

    /* Emit the output into the temporary buffer */
    ck_va_start(
         args, 
         fmt);
    len   = ck_vsnprintf(
                 buf, 
                 sizeof(buf), 
                 fmt, 
                 args);
    ck_va_end(args);
    
    /* Print formatted string in the buffer */
    str         = buf;
	if (len>UART_OUTPUT_BUF_SIZE)
		len = UART_OUTPUT_BUF_SIZE ;

	for(i=0;i<len;i++)
	//while(len)
    {
        if(*str == '\n')
        {
            // 查询UART状态寄存器，当发送数据寄存器空时，可以再次发送数据
            while( UART_IS_TX_EMPTY());
            REG_SET_VAL(rUART_SCIDR, '\r');
        }
        // 查询UART状态寄存器，当发送数据寄存器空时，可以再次发送数据
        while( UART_IS_TX_EMPTY());
		REG_SET_VAL(rUART_SCIDR,buf[i]);
		
    //    REG_SET_VAL(rUART_SCIDR,*str++);
	//	len--;
    }
	
    return ; 
}

#endif



void clear_ctrlc(void)
{
	ctrlc_was_pressed = 0;
}

