CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o squareblur.so squareblur.o

objects: squareblur.c
	$(CC) $(CCFLAGS) -o squareblur.o squareblur.c

clean: 
	- rm -f squareblur.o squareblur.so *~

