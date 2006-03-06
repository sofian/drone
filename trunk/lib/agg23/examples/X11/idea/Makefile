include ../../../Makefile.in.$(shell uname)

PROGNAME=idea
OUTNAME=$(PROGNAME)
PLATFORM=X11

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lX11

SRC=\
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp

OBJ= $(SRC:.cpp=.o)

$(PROGNAME):	$(OBJ) 
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(PROGNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -rf SunWS_cache
	rm -rf ../../SunWS_cache
	rm -rf ../../../src/platform/$(PLATFORM)/SunWS_cache

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
