TEMPLATE=lib
SOURCES+=Gear_Blur.cpp
HEADERS+=Gear_Blur.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_Blur

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Blur
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


