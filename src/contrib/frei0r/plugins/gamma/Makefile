CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared -lm

all: objects
	$(CC) $(LDFLAGS) -o gamma.so gamma.o

objects: gamma.c
	$(CC) $(CCFLAGS) -o gamma.o gamma.c

clean: 
	- rm -f gamma.o gamma.so *~

