#ifndef __COMMAND_H_
#define __COMMAND_H_

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
	int		maxargs;	/* maximum number of arguments	*/
	int		repeatable;	/* autorepeat allowed?		*/
						/* Implementation function	*/
	unsigned int (*cmd)(struct cmd_tbl_s *,int, int,char *[]);
	char		*usage;		/* Usage message	(short)	*/
};



/* common/command.c */
extern cmd_tbl_t *find_cmd(const char *cmd);
extern cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len);
extern int  register_cmd(cmd_tbl_t  *cmd_item);

extern int cmd_usage(char *name);

#ifdef CONFIG_AUTO_COMPLETE
extern void install_auto_complete(void);
extern int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp);
#endif

#define Struct_Section  __attribute__ ((unused,section (".u_boot_cmd")))

#define U_BOOT_CMD(name,maxargs,rep,cmd,usage) \
cmd_tbl_t __u_boot_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage}

#define U_BOOT_CMD_MKENT(name,maxargs,rep,cmd,usage) \
{#name, maxargs, rep, cmd, usage}

/********************************extern string***********************************/

/********************************extern uart*************************************/
extern void uart_init(void);
extern int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp);
extern void clear_ctrlc(void);
extern int had_ctrlc (void);
extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
#endif


	
