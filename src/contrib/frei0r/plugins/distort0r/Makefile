CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o distort0r.so distort0r.o

objects: distort0r.c
	$(CC) $(CCFLAGS) -o distort0r.o distort0r.c

clean: 
	- rm -f distort0r.o distort0r.so *~

