include ../../../Makefile.in.$(shell uname)

PROGNAME = image1
OUTNAME = $(PROGNAME)
PLATFORM = X11

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-I/usr/X11R6/include \
-L/usr/X11R6/lib \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lX11

SRC=\
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp


OBJ=$(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
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

