/******************************************************************************
*                   CONFIDENTIAL                                
*        Hangzhou NationalChip Science and Technology Co., Ltd.             
*                      (C)2010-2020, All right reserved
*******************************************************************************

*******************************************************************************
* File Name :   module_command.c
* Author    :   Liujh
* Project   :   
* Subpro    :   
*******************************************************************************
* Purpose   :   
*******************************************************************************
* Release History:
  VERSION   Date              AUTHOR         Description
   0.0      2010.06.04        W.L.M          creation
******************************************************************************/

/* Includes ---------------------------------------------------------------- */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autotest_command.h"

/* Private Macros --------------------------------------------------------- */


/* Exproted Macros -------------------------------------------------------- */


/* Private types/constants ------------------------------------------------ */


/* Exproted types/constants ----------------------------------------------- */


/* Private Variables ------------------------------------------------------ */


/* Exported Variables ----------------------------------------------------- */

static char console_buffer[CONFIG_SYS_CBSIZE] = {0};		/* console I/O buffer	*/
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char tab_seq[] = "        ";		/* used to expand TABs	*/

static unsigned int hist_num ;
static char* hist_list[HIST_MAX];
static char hist_lines[HIST_MAX][HIST_SIZE];

/*****************************************************************
* Function 		:delete_char
* Description 	:
* Argumentd 	:
* Return		:
* Others		:
******************************************************************/
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			puts (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				puts (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				putchar (*s);
			}
		}
	} else {
		puts (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return (p);
}


/***************************************************************************
* Function 		:process_macros
* Description	:
* Arguments		:
* Return 		:
* Others		:
****************************************************************************/
static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen ((char*)input);
	int outputcnt = CONFIG_SYS_CBSIZE;
	int state = 0;
	
	/* 0 = waiting for '$'  */
	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */


	//log_printf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),input);

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) 
	{
		c = *input++;
		inputcnt--;

		if (state != 3) 
		{
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) 
			{
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) 
		{
			case 0:	/* Waiting for (unescaped) $    */
				if((c == '\'') && (prev != '\\')) 
				{
					state = 3;
					break;
				}
				
				if((c == '$') && (prev != '\\')) 
				{
					state++;
				}
				else 
				{
					*(output++) = c;
					outputcnt--;
				}
				break;
			case 1:	/* Waiting for (        */
				if (c == '(' || c == '{') 
				{
					state++;
					varname_start = input;
				} 
				else 
				{
					state = 0;
					*(output++) = '$';
					outputcnt--;

					if (outputcnt) 
					{
						*(output++) = c;
						outputcnt--;
					}
				}
				break;
			case 2:	/* Waiting for )        */
				if (c == ')' || c == '}') 
				{
					int i;
					char envname[CONFIG_SYS_CBSIZE], *envval = 0;
					int envcnt = input - varname_start - 1;	/* Varname # of chars */

					/* Get the varname */
					for (i = 0; i < envcnt; i++) 
					{
						envname[i] = varname_start[i];
					}
					envname[i] = 0;

					/* Get its value */

					/* Copy into the line if it exists */
					if (envval != NULL)
						while ((*envval) && outputcnt) 
					{
							*(output++) = *(envval++);
							outputcnt--;
					}
					/* Look for another '$' */
					state = 0;
				}
				break;
			case 3:	/* Waiting for '        */
				if ((c == '\'') && (prev != '\\')) 
				{
					state = 0;
				} 
				else 
				{
					*(output++) = c;
					outputcnt--;
				}
				break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;
	else
		*(output - 1) = 0;

}


/***************************************************************************
* Function		:parse_line
* Description	:get command param num, command name and params
* Arguments		:[in]line
				 [in]argv[]
* Return 		: return param num -- argc
* Others		:argv[0]--name  argv[1]--params1  argv[2]--params2
****************************************************************************/
static int parse_line (char *line, char *argv[])
{
	int nargs = 0;
	//log_printf("Line: %s|\n",line);
	while (nargs < CONFIG_SYS_MAXARGS) {
		 //skip any white space 
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if(*line == '\n'){
			*line = '\0';
		}

		if (*line == '\0') {	// end of line, no more args	
			argv[nargs] = NULL;
			return (nargs);
		}

		argv[nargs++] = line;	// begin of argument string	

		// find end of string 
		while (*line && (*line != ' ') && (*line != '\t') && (*line != '\n')) {
			++line;
		}

		if(*line == '\n'){
			*line = '\0';
		}

		if (*line == '\0' ) {	// end of line, no more args	
			argv[nargs] = NULL;
			return (nargs);
		}

		*line++ = '\0';		//terminate current arg	 
	}

	log_printf("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

	return (nargs);
}


/**********************************************************************
* Function 		:readline_into_buffer
* Description 	:read command string into global array -- buffer
* Arguments 	:[in]prompt		display prompt string like =>
				 [out]buffer	string buffer for saving input chars
* Return 		:
* Others 		:
***********************************************************************/
int readline_into_buffer (const char *const prompt, char * buffer)
{
	char *p = buffer;

	char * p_buf = p;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char c;

	/* print prompt */
	if (prompt) 
	{
		plen = strlen (prompt);
		printf("%s",prompt);
	}
	col = plen;
	for (;;) 
	{	
		//c = getc();
		c = getchar();

		//*p++ = c;
		//log_printf("Get char : %c\n", c);

		/*
		* Special character handling
		*/
		switch (c)
		{
			case '\r':				/* Enter		*/
			case '\n':
				*p = '\0';
				//puts ("\r\n");
				return (p - p_buf);
			case '\0':				/* nul			*/
				continue;
			case 0x03:				/* ^C - break		*/
				p_buf[0] = '\0';	/* discard input */
				return (-1);
			case 0x15:				/* ^U - erase line	*/
				while (col > plen) 
				{
					puts (erase_seq);
					--col;
				}
				p = p_buf;
				n = 0;
				continue;
			case 0x17:				/* ^W - erase word	*/
				p=delete_char(p_buf, p, &col, &n, plen);
				while ((n > 0) && (*p != ' '))
				{
					p=delete_char(p_buf, p, &col, &n, plen);
				}
				continue;
			case 0x08:				/* ^H  - backspace	*/
			case 0x7F:				/* DEL - backspace	*/
				p=delete_char(p_buf, p, &col, &n, plen);
				continue;
			default:
				/*
			 	* Must be a normal character then
			 	*/
				if (n < CONFIG_SYS_CBSIZE-2)
				{
					if (c == '\t')
					{	/* expand TABs		*/
						#ifdef CONFIG_AUTO_COMPLETE
						/* if auto completion triggered just continue */
						*p = '\0';
						if (cmd_auto_complete(prompt, console_buffer, &n, &col))
						{
							p = p_buf + n;	/* reset */
							break;
						}
						#endif
						puts (tab_seq+(col&07));
						col += 8 - (col&07);
					}
					else
					{
						++col;		/* echo input		*/
						//putchar (c);
					}
					*p++ = c;
					++n;
					//log_printf("Console Buf: %s\n", console_buffer);
				}
				else
				{			/* Buffer full		*/
					putchar ('\a');
				}
		}

	}

}

/********************************************************************
* Function		:char_convert
* Description	:
* Arguments		:[in]buffer       input command string
				 [out]lastcommand formatted command string
* Return 		:
* Others		:
********************************************************************/
int char_convert(char * lastcommand, char * buffer)
{
	volatile char *p = buffer;
	int i;
	int j;
	int len = strlen(buffer);

//	return chip_error_no_error;	
	
	for(j = 0;j < len;j++)
	{
		if(((buffer[j]>='a')&&(buffer[j]<='z'))
			||((buffer[j]>='0')&&(buffer[j]<='9'))
			||(buffer[j] == '_')
			||(buffer[j] == '(')
			||(buffer[j] == ')')
			||(buffer[j] == ',')
			||(buffer[j] == ';')
			||(buffer[j] == ' '))
		{		
			continue;
		}	
		else
		{
			return chip_error_cmd_error;
		}
	}

	if(*(p+len-1) != ')')
	{
	//	*(p+len) = ')';
	//	len += 1;
		*(p+len) = '\n';
	}
	else
	{
		*(p+len) = '\n';
	}

	for(i = 0;*p != '\n';p++)
	{
		switch(*p)
		{
			case '_':
				p++;
				do{
					lastcommand[i] = *p;
					i++;
					p++;
				}while((*p != '_')&&(*p != '\n'));
				if(*p == '_')
				{
					lastcommand[i] = ' ';
					i++;
					continue;
				}
				else if(*p == '\n')
				{
					break;
				}
			case '(':
				p++;
				do{
					lastcommand[i] = *p;
					i++;
					p++;
				}while((*p != ',')&&(*p != ')'));
				if(*p == ',')
				{
					lastcommand[i] = ' ';
					i++;
					p--;
					continue;
				}
				else if((*p == ')')&&(*(p++) == '\n'))
				{
					break;
				}
				else
				{
					p--;
					continue;
				}
				
			case ',':
				p++;
				do{
					lastcommand[i] = *p;
					i++;
					p++;
				}while((*p != ',')&&(*p != ')'));
				if(*p == ',')
				{
					lastcommand[i] = ' ';
					i++;
					p--;
					continue;
				}
				else if((*p == ')')&&(*(p++) == '\n'))
				{
					break;
				}
				else
				{
					p--;
					continue;
				}
			case ';':
				p++;
				lastcommand[i] = ';';
				i++;
			default:
				continue;
		}
	}
	return chip_error_no_error;
}

/********************************************************************
* Function 		:readline
* Description	:read data from console into buffer
* Arguments		:[in]prompt	
* Return 		:
* Others		:
*********************************************************************/
int readline (const char *const prompt)
{
	/*
	 * If console_buffer isn't 0-length the user will be prompted to modify
	 * it instead of entering it from scratch as desired.
	 */
	console_buffer[0] = '\0';
	//log_printf("prompt = %c\n",prompt);
	//log_printf("console_buffer = %c\n",console_buffer[0]);
	return readline_into_buffer(prompt, console_buffer);
}


/***************************************************************
* Function 		:run_command
* Description 	: parse command and run
* Arguments 	:[in]cmd
				 [in]flag
* Return 		: rc 
* Others 		:
****************************************************************/
int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CONFIG_SYS_CBSIZE] = {0};	/* working copy of cmd		*/
	char *token;					/* start of token in cmdbuf	*/
	char *sep;						/* end of token (separator) in cmdbuf */
	char finaltoken[CONFIG_SYS_CBSIZE] = {0};
	char *str = cmdbuf;
	char *argv[CONFIG_SYS_MAXARGS + 1] = {0};	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

	unsigned int	value_return = 0;


	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CONFIG_SYS_CBSIZE) {
		puts ("## Command too long!\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	while (*str) {
		// divide batch commands into single command according to ";"
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */


		/* find macros in this token and replace them */
		process_macros (token, finaltoken);
		//log_printf("[] [%s]\n",finaltoken);

		/* Extract arguments */
		/*argc is the count of command params*/
		/*argv is the */
		if ((argc = parse_line (finaltoken, argv)) == 0) {
			rc = -1;	/* no command at all */
			log_printf ("<=[AutoTestCommand]: no command at all. \n");
			continue;
		}
		//log_printf("argc = %d\n", argc);

		/* Look up command in command table */
		//log_printf("%s\n",argv[0]);
		//log_printf("%s\n",argv[1]);
		cmdtp = find_cmd(argv[0]);
		if (cmdtp == NULL) {
			log_printf ("<=[AutoTestCommand]: unknown command - [%s]\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}
		
		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			//cmd_usage(cmdtp);
			log_printf("<=[AutoTestCommand]: find command but param error!\n");
			rc = -1;
			continue;
		}

		
		/* OK - call function to do the command */
		
		if ( cmdtp->cmd != NULL)
		{
			value_return = (cmdtp->cmd) (cmdtp, flag, argc, argv);

			rc = -1;

			log_printf("<=[AutoTestCommand]: return = %d\n",value_return);
			//log_printf("gxluar1\n");
		}
		

		repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
			return -1;	/* if stopped then not repeatable */
	}
	
		
	return rc ? rc : repeatable;
}


/*******************************************************************
* Function 		:main_loop
* Description 	:read data from uart , parse command
* Arguments 	:[in]void
* Return 		:void
* Others 		:
********************************************************************/
void command_main_loop(void)
{
	static char lastcommand[CONFIG_SYS_CBSIZE] = {0};
	int error;
	int len;
	int rc = 1;
	int flag;

	while(1)
	{
		memset((void *)lastcommand, 0, CONFIG_SYS_CBSIZE);
		memset((void *)console_buffer,0,CONFIG_SYS_CBSIZE);
		/*read command from console*/
		len = readline (CONFIG_SYS_PROMPT);
		/*convert command into string divided by space */
		error = char_convert(lastcommand,console_buffer);
		if(error == chip_error_no_error)
		{
			flag = 0;	
			
			if (len == 0)
			{
				flag |= CMD_FLAG_REPEAT;
			}
		
			if (len == -1)
			{
				puts ("<INTERRUPT>\n");
			}
			else
			{
				//rc = run_command (lastcommand, flag);
				//log_printf("Console Buffer: <%s>\n", console_buffer);
				rc = run_command (console_buffer, flag);
			}
			
			if (rc <= 0)
			{		
				lastcommand[0] = 0;
			}
		}
		else if(error == chip_error_cmd_error)
		{
			log_printf("The command is wrong !Please put again !\n");
			continue;
		}
	}
}

int main(void)
{

	// open console uart
	// uart_init();
	
	// register commands list
	extern cmd_tbl_t cmd_set_reg;
	extern cmd_tbl_t cmd_get_reg;

	log_printf("register cmd begin.\n");
	register_cmd(&cmd_set_reg);
	register_cmd(&cmd_get_reg);
	log_printf("register cmd finish.\n");

	command_main_loop();
}
