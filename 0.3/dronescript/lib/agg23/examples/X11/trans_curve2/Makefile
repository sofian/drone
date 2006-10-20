include ../../../Makefile.in.$(shell uname)

PROGNAME=trans_curve2_ft
OUTNAME=$(PROGNAME)
PLATFORM=X11

CXXFLAGS= $(AGGCXXFLAGS) \
-I../../../include \
-I../../../font_freetype \
-I/usr/local/include/freetype2 \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lX11 -lfreetype

SRC=\
../../$(PROGNAME).cpp \
../../interactive_polygon.cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../../font_freetype/agg_font_freetype.cpp 

OBJ= $(SRC:.cpp=.o)

$(PROGNAME):	$(OBJ) 
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(PROGNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../interactive_polygon.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -rf SunWS_cache
	rm -rf ../../SunWS_cache
	rm -rf ../../../src/platform/$(PLATFORM)/SunWS_cache
	

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
