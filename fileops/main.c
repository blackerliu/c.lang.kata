/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年03月02日 15时55分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  Hangzhou Nationalchip Science&Technology Co.Ltd.
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <poll.h>
#include <libgen.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#define MAIN_APP	"file_utils"

#define CMD_FILE_READ	"file_read"
#define CMD_FILE_WRITE	"file_write"

#define BUF_SIZE	(1024)
#define SIZE_KB		(1024)
#define FILE_SIZE	(256 * 1024 * 1024)

static void print_usage(void);
static int  check_cmd(int argc, char **argv, char *cmd);


static int cmd_file_read(int argc, char **argv);
static int cmd_file_write(int argc, char **argv);

int main(int argc, char **argv)
{

	if(argc == 1)
	{
		if(strncmp(basename(argv[0]), MAIN_APP, sizeof(MAIN_APP)) == 0)
		{
			print_usage();
			return 0;
		}
	}

	if(check_cmd(argc, argv, CMD_FILE_READ) == 0)
	{
		printf("File Read.\n");
	        cmd_file_read(argc, argv);
		return 0;
	}

	if(check_cmd(argc, argv, CMD_FILE_WRITE) == 0)
	{
		printf("File Write.\n");
		cmd_file_write(argc, argv);
		return 0;
	}
	

	return 0;
}

static void print_usage(void)
{
		printf("Usage: \n");
		printf("ln -s file_utils file_read\n");
		printf("ln -s file_utils file_write\n");
		printf("Run:\n");
		printf("command [options].\n");
		printf("now supportted command: file_read, file_write.\n");
		
		return;
}

#define debug_print() 	printf("argc:%d, argv[0]: %s\n", argc, argv[0])

static int check_cmd(int argc, char **argv, char *cmd)
{
	char *pcmd = cmd;

	 if(((strncmp(basename(argv[0]), MAIN_APP,strlen(MAIN_APP)) == 0) && argc > 1 && (strncmp(argv[1], pcmd,strlen(pcmd)) == 0)) \
	    ||(strncmp(basename(argv[0]), pcmd, strlen(pcmd)) == 0))
	 {
	
	 	return 0;
	 }
	
	return -1;
}

static int cmd_file_read(int argc, char **argv)
{
	int fd;
static	char buffer[BUF_SIZE];
	char *ptr;
	int bytes_num;
	int counter = 0;
	time_t time_start, time_end;
	unsigned int speed;
	
	
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s frome_file_name\n", argv[0]);
		exit(1);
	}
	
	if((fd = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stderr, "Open file [%s] error [%s]\n", argv[1], strerror(errno));
		exit(1);
	}
	
	printf("Begin to Read File: %s\n", argv[1]);
	
	counter = 0;
	
	time_start = time(NULL);
	while(bytes_num = read(fd, buffer, BUF_SIZE))
	{
		counter += bytes_num;
	}
	
	time_end = time(NULL);
	
	close(fd);
	
	speed = counter / difftime(time_end, time_start)/SIZE_KB;
	
	printf("File Read Speed : %d KB/s\n", speed);
	
	return 0;
}

static int cmd_file_write(int argc, char **argv)
{
	int fd;
static	char buffer[BUF_SIZE];
	char *ptr;
	int bytes_num;
	int counter = 0;
	time_t time_start, time_end;
	unsigned int speed;
	int i;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s to_file_name\n", argv[0]);
		exit(1);
	}
	
	if((fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	{
		fprintf(stderr, "Open file [%s] error [%s]\n", argv[1], strerror(errno));
		exit(1);
	}
	
	
	/* init buffer */
	for(i = 0; i < BUF_SIZE; i++)
	{
		ptr = buffer + i;
		*(unsigned int *)ptr = 0xDEADBEEF;
		i += sizeof(*ptr);
	}
	printf("Begin to Write File: %s\n", argv[1]);
	
	time_start = time(NULL);
	counter = 0;
	
	while( counter < FILE_SIZE)
	{
		if((bytes_num = write(fd, buffer, BUF_SIZE)) == -1)
		{
			fprintf(stderr, "Write file error.\n");
			break;
		}
		
		counter += bytes_num;
	}

	//fflush(fd);
	
	close(fd);	
	time_end = time(NULL);
	
	speed = counter / difftime(time_end, time_start)/SIZE_KB;
	
	printf("File Write Speed : %d KB/s\n", speed);
	
	return 0;
}

