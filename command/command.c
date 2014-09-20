//#include <stdio.h>
//#include <string.h>

#include "stddef.h"
#include "command.h"

#include "command_resolve.h"

static char tmp_buf[CONFIG_SYS_CBSIZE];	/* copy of console I/O buffer	*/

cmd_tbl_t  __command_list[MAX_COMMAND_NUM];
unsigned int __command_list_used = 0;

/*************************************************************************
* Function		:print_argv
* Description	:
* Arguments		:
* Return		:
* Others		:
*************************************************************************/
static void print_argv(const char *banner, const char *leader, const char *sep, int linemax, char *argv[])
{
	int ll = leader != NULL ? strlen(leader) : 0;
	int sl = sep != NULL ? strlen(sep) : 0;
	int len, i;

	if (banner) {
		puts("\n");
		puts(banner);
	}

	i = linemax;	/* force leader and newline */
	while (*argv != NULL) {
		len = strlen(*argv) + sl;
		if (i + len >= linemax) {
			puts("\n");
			if (leader)
				puts(leader);
			i = ll - sl;
		} else if (sep)
			puts(sep);
		puts(*argv++);
		i += len;
	}
	printf("\n");
}


/*************************************************************************
* Function		:find_common_prefix
* Description	:
* Arguments		:
* Return		:
* Others		:
*************************************************************************/
static int find_common_prefix(char *argv[])
{
	int i, len;
	char *anchor, *s, *t;

	if (*argv == NULL)
		return 0;

	/* begin with max */
	anchor = *argv++;
	len = strlen(anchor);
	while ((t = *argv++) != NULL) {
		s = anchor;
		for (i = 0; i < len; i++, t++, s++) {
			if (*t != *s)
				break;
		}
		len = s - anchor;
	}
	return len;
}


/*************************************************************************
* Function		:make_argv
* Description	:
* Arguments		:
* Return		:
* Others		:
*************************************************************************/
static int make_argv(char *s, int argvsz, char *argv[])
{
	int argc = 0;

	/* split into argv */
	while (argc < argvsz - 1) {

		/* skip any white space */
		while ((*s == ' ') || (*s == '\t'))
			++s;

		if (*s == '\0')	/* end of s, no more args	*/
			break;

		argv[argc++] = s;	/* begin of argument string	*/

		/* find end of string */
		while (*s && (*s != ' ') && (*s != '\t'))
			++s;

		if (*s == '\0')		/* end of s, no more args	*/
			break;

		*s++ = '\0';		/* terminate current arg	 */
	}
	argv[argc] = NULL;

	return argc;
}

/***************************************************************************
* Function		:complete_cmdv
* Description	:
* Arguments		:
* Return		:
* Others		:
***************************************************************************/
static int complete_cmdv(int argc, char *argv[], char last_char, int maxv, char *cmdv[])
{
	cmd_tbl_t *cmdtp;
	const char *p;
	int len, clen;
	int n_found = 0;
	const char *cmd;

	/* sanity? */
	if (maxv < 2)
		return -2;

	cmdv[0] = NULL;

	if (argc == 0) {
		/* output full list of commands */
		//for (cmdtp = &__u_boot_cmd_start; cmdtp != &__u_boot_cmd_end; cmdtp++) {
		for (cmdtp = __command_list; cmdtp != __command_list + __command_list_used; cmdtp++) {
			if (n_found >= maxv - 2) {
				cmdv[n_found++] = "...";
				break;
			}
			cmdv[n_found++] = cmdtp->name;
		}
		cmdv[n_found] = NULL;
		return n_found;
	}

	/* more than one arg or one but the start of the next */
	if (argc > 1 || (last_char == '\0' || last_char == ' ' || last_char == '\t')) {
		cmdtp = find_cmd(argv[0]);
		if (cmdtp == NULL ) {
			cmdv[0] = NULL;
			return 0;
		}
	}

	cmd = argv[0];
	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	p = strchr(cmd, '.');
	if (p == NULL)
		len = strlen(cmd);
	else
		len = p - cmd;

	/* return the partial matches */
	//for (cmdtp = &__u_boot_cmd_start; cmdtp != &__u_boot_cmd_end; cmdtp++) {
	for (cmdtp = __command_list; cmdtp != __command_list + __command_list_used; cmdtp++) {

		clen = strlen(cmdtp->name);
		if (clen < len)
			continue;

		if (memcmp(cmd, cmdtp->name, len) != 0)
			continue;

		/* too many! */
		if (n_found >= maxv - 2) {
			cmdv[n_found++] = "...";
			break;
		}

		cmdv[n_found++] = cmdtp->name;
	}

	cmdv[n_found] = NULL;
	return n_found;
}

/***************************************************************************
 * Function 	:find_cmd_tbl
 * Description 	:
 * Arguments 	:
 * Return 		:
 * Others 		:
 **************************************************************************/
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len)
{
	cmd_tbl_t *cmdtp;
	cmd_tbl_t *cmdtp_temp = table;	/*Init value */
	const char *p;
	int len;
	int n_found = 0;

	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	len = ((p = strchr(cmd, '.')) == NULL) ? strlen (cmd) : (p - cmd);

	for (cmdtp = table;
	     cmdtp != table + table_len;
	     cmdtp++) {
		//if (strncmp (cmd, cmdtp->name, len) == 0) {
		if (strcmp (cmd, cmdtp->name) == 0) {
			if (len == strlen (cmdtp->name))
				return cmdtp;	/* full match */

			cmdtp_temp = cmdtp;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1) {			/* exactly one match */
		return cmdtp_temp;
	}

	return NULL;	/* not found or ambiguous command */
}

cmd_tbl_t *find_cmd (const char *cmd)
{
	
	//int len = (unsigned int *)((unsigned char *)&__u_boot_cmd_end - (unsigned char *)&__u_boot_cmd_start);
	int len = __command_list_used; 
	
	//return find_cmd_tbl(cmd, &__u_boot_cmd_start, len);
	return find_cmd_tbl(cmd, __command_list, len);
}

int register_cmd(cmd_tbl_t *cmd_item)
{
	cmd_tbl_t *cmdtp;
	cmd_tbl_t *cmdnew;

	int len = 0;
	const char *p;

	if( __command_list_used + 1 >= MAX_COMMAND_NUM){
		log_printf("[TestCommand] command list is full when registering.\n");
		return -1;
	}

	//printf("name:%s  index:%d\n",cmd_item->name,__command_list_used);

	//len = ((p = strchr(cmd_item->name, '.')) == NULL) ? strlen (cmd_item->name) : (p - cmd_item->name);
	len = strlen(cmd_item->name);

//	printf("cmd len: %d\n", len);
	for(cmdtp = __command_list; cmdtp != &__command_list[__command_list_used]; cmdtp++){

		if(cmdtp->name == NULL){
			break;
		}
		//if (strncmp (cmd_item->name, cmdtp->name, len) == 0) {
		if (strcmp (cmd_item->name, cmdtp->name) == 0) {
			log_printf("[TestCommand] find same command name when registering.\n");
			return -1;
		}
	}
	
	cmdnew = __command_list + __command_list_used;
	memcpy(cmdnew, cmd_item, sizeof(cmd_tbl_t));
	log_printf("Register command :%s\n",cmdnew->name );

	__command_list_used++;

	return 0;
}

int cmd_usage(char *name)
{
	printf("%s - %s\n\n", name, name);
	return 0;
}


int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp)
{
	int n = *np, col = *colp;
	char *argv[CONFIG_SYS_MAXARGS + 1];		/* NULL terminated	*/
	char *cmdv[20];
	char *s, *t;
	const char *sep;
	int i, j, k, len, seplen, argc;
	int cnt;
	char last_char;

	if (strcmp(prompt, CONFIG_SYS_PROMPT) != 0)
		return 0;	/* not in normal console */

	cnt = strlen(buf);
	if (cnt >= 1)
		last_char = buf[cnt - 1];
	else
		last_char = '\0';

	/* copy to secondary buffer which will be affected */
	strcpy(tmp_buf, buf);

	/* separate into argv */
	argc = make_argv(tmp_buf, sizeof(argv)/sizeof(argv[0]), argv);

	/* do the completion and return the possible completions */
	i = complete_cmdv(argc, argv, last_char, sizeof(cmdv)/sizeof(cmdv[0]), cmdv);

	/* no match; bell and out */
	if (i == 0) {
		if (argc > 1)	/* allow tab for non command */
			return 0;
		putc('\a');
		return 1;
	}

	s = NULL;
	len = 0;
	sep = NULL;
	seplen = 0;
	if (i == 1) { /* one match; perfect */
		k = strlen(argv[argc - 1]);
		s = cmdv[0] + k;
		len = strlen(s);
		sep = " ";
		seplen = 1;
	} else if (i > 1 && (j = find_common_prefix(cmdv)) != 0) {	/* more */
		k = strlen(argv[argc - 1]);
		j -= k;
		if (j > 0) {
			s = cmdv[0] + k;
			len = j;
		}
	}

	if (s != NULL) {
		k = len + seplen;
		/* make sure it fits */
		if (n + k >= CONFIG_SYS_CBSIZE - 2) {
			putc('\a');
			return 1;
		}

		t = buf + cnt;
		for (i = 0; i < len; i++)
			*t++ = *s++;
		if (sep != NULL)
			for (i = 0; i < seplen; i++)
				*t++ = sep[i];
		*t = '\0';
		n += k;
		col += k;
		puts(t - k);
		if (sep == NULL)
			putc('\a');
		*np = n;
		*colp = col;
	} else {
		print_argv(NULL, "  ", " ", 78, cmdv);

		puts(prompt);
		puts(buf);
	}
	return 1;
}

