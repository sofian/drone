include ../../../Makefile.in.$(shell uname)

PROGNAME=trans_curve1_ft
OUTNAME=$(PROGNAME)
PLATFORM=BeOS

CXXFLAGS= $(AGGCXXFLAGS) \
-I../../../include \
-I../../../font_freetype \
-I/boot/home/config/include/ \
-L../../../src \
-L/boot/home/config/lib/ \
$(PIXFMT)

LIBS = $(AGGLIBS) -lroot -lbe -ltranslation -lfreetype

SRC=\
../../$(PROGNAME).cpp \
../../interactive_polygon.cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../../font_freetype/agg_font_freetype.cpp 

OBJ= $(SRC:.cpp=.o)

$(PROGNAME):	$(OBJ) 
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)
	mimeset -f $(OUTNAME)

clean:
	rm -f $(PROGNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
