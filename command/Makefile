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
	$(CC) -Iinclude -g -static -o main main.c command.c console.c cmd_reg.c 

main.elf : $(COBJ) 
	$(LD)  $^ -o $@ \
 

%.o : %.c	
	$(CC) $(CFLAGS) $< -o $@

%.o : %.S
	$(CC) $(CFLAGS) $< -o $@

-include $(subst .c,.d,$(SOURCE))
-include $(subst .S,.d,$(SSOURCE))

#%.d : %.c
#	$(CC) $(CPPFLAGS) -M $< > $@.$$$$;\
#	sed 's,\($*\)\.o[:]*,\1.o $@ : ,g' <$@.$$$$ > $@;\
#	rm -f $@.$$$$

#%.d : %.S
#	$(CC) $(CPPFLAGS) -M $< > $@.$$$$;\
#	sed 's,\($*\)\.o[:]*,\1.o $@ : ,g' <$@.$$$$ > $@;\
#	rm -f $@.$$$$

.PHONY:clean 
clean:
	rm -f *.o *.elf *.d *.txt main


