TEMPLATE=lib
SOURCES+=Gear_ColorAdjust.cpp
HEADERS+=Gear_ColorAdjust.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_ColorAdjust

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_ColorAdjust
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


