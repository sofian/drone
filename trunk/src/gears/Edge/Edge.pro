include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Edge.cpp
HEADERS+=Gear_Edge.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Edge

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Edge
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


