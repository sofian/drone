OS := $(shell uname -s)
TARGET := drone_frei0r_jni_Frei0r

include Makefile_options_common
include Makefile_options_$(OS)

JNILIB := lib$(TARGET).$(EXT)
SOURCES := $(TARGET).cpp

all:
	@echo ">> Trying to compile" $(TARGET) "for" $(OS) " <<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(JNILIB) $(SOURCES)
	@mv $(JNILIB) lib/
	@echo ">> done <<"

clean:
	@echo ">> Cleaning up <<"
	\rm -f core *.o lib/$(JNILIB)
	@echo ">> done <<"

Makefile_options_$(OS):
	@echo "**** PLEASE CHECK YOUR MAKEFILE_OPTIONS for >>" $(OS) "<< ****"
