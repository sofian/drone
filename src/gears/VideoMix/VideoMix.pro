TEMPLATE=lib
SOURCES+=Gear_VideoMix.cpp
HEADERS+=Gear_VideoMix.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_VideoMix

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoMix
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


