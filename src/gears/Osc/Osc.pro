TEMPLATE=lib
SOURCES+=Gear_Osc.cpp
HEADERS+=Gear_Osc.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_Osc

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Osc
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

