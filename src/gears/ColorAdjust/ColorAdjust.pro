TEMPLATE=lib
SOURCES+=Gear_ColorAdjust.cpp
HEADERS+=Gear_ColorAdjust.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_ColorAdjust

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_ColorAdjust
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


