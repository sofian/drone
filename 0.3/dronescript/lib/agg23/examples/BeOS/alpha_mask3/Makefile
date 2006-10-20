include ../../../Makefile.in.$(shell uname)

PROGNAME=alpha_mask3
OUTNAME=$(PROGNAME)
PLATFORM=BeOS

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include -I../../../gpc \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lroot -lbe -ltranslation

SRC_CXX = \
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../make_arrows.cpp \
../../make_gb_poly.cpp

OBJ = $(SRC_CXX:.cpp=.o)

$(PROGNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)
	mimeset -f $(OUTNAME)

clean:
	rm -f $(PROGNAME)
	rm -f *.o
	rm -f ../../*.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@

