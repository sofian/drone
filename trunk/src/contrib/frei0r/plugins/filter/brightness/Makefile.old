CC=gcc
CCFLAGS=-Wall -fPIC -g -I../../ -c
LDFLAGS=-Wl -shared

all: objects
	$(CC) $(LDFLAGS) -o ../../../../../gears/libbrightness.so brightness.o

objects: brightness.c
	$(CC) $(CCFLAGS) -o brightness.o brightness.c

clean: 
	- rm -f brightness.o ../../../../../gears/libbrightness.so *~

