TEMPLATE=lib
SOURCES+=Gear_VideoOutput.cpp VideoOutputGl.cpp VideoOutputMaker.cpp VideoOutputShm.cpp VideoOutputX11Base.cpp VideoOutputXv.cpp
HEADERS+=Gear_VideoOutput.h VideoOutput.h VideoOutputGl.h VideoOutputMaker.h VideoOutputShm.h VideoOutputX11Base.h VideoOutputXv.h
INCLUDEPATH+=../../core/ ../../core/types
LIBS+= -L../../../lib/ -L/usr/X11R6/lib/ -ldroneCore -lGL -lXv -lGLU -lXxf86vm 
TARGET=../../../gears/Gear_VideoOutput


