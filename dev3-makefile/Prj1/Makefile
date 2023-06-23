CC=gcc
CFLAGS= -c -Wall

all: prog

prog: main.o hello.o functions/functions.o
	$(CC) main.o hello.o functions/functions.o -o prog

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o
	
hello.o: hello.c
	$(cc) $(CFLAGS)hello.c -o hello.o

functions/functions: functions/functions.cc
	$(CC) $(CFLAGS) functions/functions.c -o functions/functions.o

.PHONY: clean

clean:
	rm -rf *.o functions/*.o
	rm -rf prog