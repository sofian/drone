TEMPLATE=lib
SOURCES+=Gear_AreaArrayMask.cpp
HEADERS+=Gear_AreaArrayMask.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_AreaArrayMask

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaArrayMask
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


