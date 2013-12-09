CC=csky-linux-gcc

all: dynlib bin
	$(CC) -g -o main main.c -static

dynlib:
	$(CC) -c funlib.c 

bin: dynlib
	@./make_bin.sh external_customer
#	./make_bin.sh external_sub

clean:
	rm -f funlib.o funcode.bin main
