TEMPLATE=lib
SOURCES+=Gear_Edge.cpp
HEADERS+=Gear_Edge.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_Edge

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Edge
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


