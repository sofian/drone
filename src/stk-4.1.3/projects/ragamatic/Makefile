### STK ragamatic Makefile - for various flavors of unix

PROGRAMS = ragamat
RM = /bin/rm
SRC_PATH = ../../src
OBJECT_PATH = Debug
vpath %.o $(OBJECT_PATH)

OBJECTS	=	Stk.o Envelope.o ADSR.o Noise.o \
					Filter.o DelayA.o Delay.o \
					OnePole.o OneZero.o SKINI.o \
					Tabla.o Instrmnt.o Sitar.o \
					Drone.o VoicDrum.o WvOut.o WvIn.o \
					Reverb.o JCRev.o Messager.o

INCLUDE = 
ifeq ($(strip $(INCLUDE)), )
	INCLUDE = ../../include
endif
vpath %.h $(INCLUDE)

CC       = g++
DEFS     = -D__LITTLE_ENDIAN__
DEFS    += -D_STK_DEBUG_
CFLAGS   = -g
CFLAGS  += -Wall -g -I$(INCLUDE)
LIBRARY = -lasound -lpthread -lm 
LIBRARY += 

REALTIME = yes
ifeq ($(REALTIME),yes)
	OBJECTS += RtMidi.o RtAudio.o RtWvOut.o Thread.o Socket.o
	DEFS    += -D__LINUX_ALSA__ 
	DEFS    += 
endif

RAWWAVES = 
ifeq ($(strip $(RAWWAVES)), )
	RAWWAVES = ../../rawwaves/
endif
DEFS    += -DRAWWAVE_PATH=\"$(RAWWAVES)\"

%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

all : $(PROGRAMS)

ragamat: ragamat.cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(DEFS) -o ragamat ragamat.cpp $(OBJECT_PATH)/*.o $(LIBRARY)

$(OBJECTS) : Stk.h

clean : 
	-rm $(OBJECT_PATH)/*.o
	-rm $(PROGRAMS)

strip : 
	strip $(PROGRAMS)

# Project specific objects:

Tabla.o: Tabla.cpp
	$(CC) $(CFLAGS) $(DEFS) -c Tabla.cpp -o $(OBJECT_PATH)/$@

Drone.o: Drone.cpp
	$(CC) $(CFLAGS) $(DEFS) -c Drone.cpp -o $(OBJECT_PATH)/$@

VoicDrum.o: VoicDrum.cpp
	$(CC) $(CFLAGS) $(DEFS) -c VoicDrum.cpp -o $(OBJECT_PATH)/$@
