TEMPLATE=lib
SOURCES+=Gear_VideoSource.cpp
HEADERS+=Gear_VideoSource.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/linux/libmpeg3/include
LIBS+=-L../../../lib/ -L../../../lib/linux/libmpeg3/ -ldroneCore -lmpeg3
TARGET=../../../gears/Gear_VideoSource


