CC=gcc

lib:
	$(CC) fun1.c -c -o fun1.o 
	$(CC) fun2.c -c -o fun2.o
	$(CC) fun3.c -c -o fun3.o
	$(CC) test.c -c -o test.o
	ar rcs libfun1.a fun1.o
	ar rcs libfun2.a fun2.o
	ar rcs libfun3.a fun3.o

ld:
	$(CC) test.o -L./ -lfun2 -lfun1  -lfun3 -o test

error:
	$(CC) test.o -L./ -lfun1 -lfun2 -lfun3 -o test


clean:
	rm -f test *.o *.a

