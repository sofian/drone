CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o contrast0r.so contrast0r.o

objects: contrast0r.c
	$(CC) $(CCFLAGS) -o contrast0r.o contrast0r.c

clean: 
	- rm -f contrast0r.o contrast0r.so *~

