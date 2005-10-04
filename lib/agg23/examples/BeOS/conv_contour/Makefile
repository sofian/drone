include ../../../Makefile.in.$(shell uname)

PROGNAME = conv_contour
OUTNAME = $(PROGNAME)
PLATFORM=BeOS

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lroot -lbe -ltranslation

SRC=\
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp


OBJ=$(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)
	mimeset -f $(OUTNAME)

clean:
	rm -f $(PROGNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@

