### STK effects Makefile - for various flavors of unix

PROGRAMS = effects
RM = /bin/rm
SRC_PATH = ../../src
OBJECT_PATH = Debug
vpath %.o $(OBJECT_PATH)

OBJECTS	=	Stk.o Reverb.o PRCRev.o JCRev.o \
					NRev.o Delay.o Filter.o \
					SKINI.o Envelope.o Echo.o \
					PitShift.o DelayL.o Chorus.o \
					WvIn.o WaveLoop.o Messager.o

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
	OBJECTS += RtMidi.o RtAudio.o RtDuplex.o Thread.o Socket.o
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

effects: effects.cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(DEFS) -o effects effects.cpp $(OBJECT_PATH)/*.o $(LIBRARY)

$(OBJECTS) : Stk.h

clean : 
	-rm $(OBJECT_PATH)/*.o
	-rm $(PROGRAMS)

strip : 
	strip $(PROGRAMS)
