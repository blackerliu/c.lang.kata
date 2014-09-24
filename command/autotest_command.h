#ifndef __AUTOTEST_COMMAND_H_
#define __AUTOTEST_COMMAND_H_

#define CONFIG_SYS_PROMPT       "=>"            /* Monitor Command Prompt       */
#define CMD_FLAG_REPEAT         0x0001          /* repeat last command          */
#define CONFIG_SYS_CBSIZE                       256
#define CTL_BACKSPACE           ('\b')
#define MAX_CMDBUF_SIZE         256
#define CONFIG_SYS_MAXARGS              16
#define GD_FLG_RELOC    0x00001         	/* Code was relocated to RAM            */

#define HIST_MAX                20
#define HIST_SIZE               MAX_CMDBUF_SIZE

#define NAMESIZE 16
#define CTLRSIZE 8
#define MAX_COMMAND_NUM 	(32)

#ifndef true
    #define true    (1 == 1)
#endif
#ifndef false
    #define false   (!true)
#endif

#define log_printf	printf

enum
{
        chip_error_no_error = 0,
        chip_error_cmd_error
};


struct serial_device {
	char name[NAMESIZE];
	char ctlr[CTLRSIZE];

	int  (*init) (void);
	void (*setbrg) (void);
	int (*getc) (void);
	int (*tstc) (void);
	void (*putc) (const char c);
	void (*puts) (const char *s);

	struct serial_device *next;
};

//-------
typedef struct cmd_tbl_s	cmd_tbl_t;

struct cmd_tbl_s {
	char	*name;		/* Command Name			*/
	int	 maxargs;	/* maximum number of arguments	*/
	int	 repeatable;	/* autorepeat allowed?		*/
				/* Implementation function	*/
	unsigned int (*cmd)(struct cmd_tbl_s *,int, int,char *[]);
	char	*usage;		/* Usage message	(short)	*/
};



/* common/command.c */
extern cmd_tbl_t *find_cmd(const char *cmd);
extern cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len);
extern int  register_cmd(cmd_tbl_t  *cmd_item);
extern int  unregister_cmd(cmd_tbl_t *cmd_item);

/********************************extern string***********************************/

/********************************extern uart*************************************/
extern void uart_init(void);
extern void clear_ctrlc(void);
extern int had_ctrlc (void);

#endif


	
