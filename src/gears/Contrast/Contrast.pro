TEMPLATE=lib
SOURCES+=Gear_Contrast.cpp
HEADERS+=Gear_Contrast.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_Contrast

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Contrast
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

