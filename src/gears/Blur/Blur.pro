TEMPLATE=lib
SOURCES+=Gear_Blur.cpp
HEADERS+=Gear_Blur.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_Blur

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Blur
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


