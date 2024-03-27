CC=gcc
CGLAGS=-Wall

.PHONY: all clean start

all: lab5

lab5: lab5.o
	$(CC) $(CFLAGS) lab5.o -o lab5
	
start: lab5
	sudo ./lab5
	
clean:
	rm -f lab5 lab5.o 
