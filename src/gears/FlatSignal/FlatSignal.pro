TEMPLATE=lib
SOURCES+=Gear_FlatSignal.cpp
HEADERS+=Gear_FlatSignal.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_FlatSignal

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_FlatSignal
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


