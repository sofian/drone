CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o saturat0r.so saturat0r.o

objects: saturat0r.c
	$(CC) $(CCFLAGS) -o saturat0r.o saturat0r.c

clean: 
	- rm -f saturat0r.o saturat0r.so *~

