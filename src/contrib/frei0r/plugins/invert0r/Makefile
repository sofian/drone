CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o invert0r.so invert0r.o

objects: invert0r.c
	$(CC) $(CCFLAGS) -o invert0r.o invert0r.c

clean: 
	- rm -f invert0r.o invert0r.so *~

