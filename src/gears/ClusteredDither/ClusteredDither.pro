TEMPLATE=lib
SOURCES+=Gear_ClusteredDither.cpp
HEADERS+=Gear_ClusteredDither.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_ClusteredDither

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_ClusteredDither
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


