TEMPLATE=lib
SOURCES+=Gear_Brightness.cpp
HEADERS+=Gear_Brightness.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_Brightness

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Brightness
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


