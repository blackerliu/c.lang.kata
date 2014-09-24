CC = gcc
AS = as
LD = ld

SOURCE = $(shell find ./ -name "*.c")
SSOURCE = $(shell find ./ -name "*.S")
COBJ = $(subst .c,.o,$(SOURCE))
SOBJ = $(subst .S,.o,$(SSOURCE))

CPPFLAGS = -I include

CFLAGS  =  $(CPPFLAGS)  -r -c  -O0 -g

all: 
	$(CC) -g -static -o main common_autotest.c autotest_command.c autotest_console.c command_reg.c 

main.elf : $(COBJ) 
	$(LD)  $^ -o $@ \
 

%.o : %.c	
	$(CC) -g $(CFLAGS) $< -o $@

-include $(subst .c,.d,$(SOURCE))
-include $(subst .S,.d,$(SSOURCE))

.PHONY:clean 
clean:
	rm -f *.o *.elf *.d *.txt main


