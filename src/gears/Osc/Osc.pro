TEMPLATE=lib
SOURCES+=Gear_Osc.cpp
HEADERS+=Gear_Osc.h
INCLUDEPATH+=../../core/ ../../core/types
linux:LIBS+=-L../../../lib/ -ldroneCore
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
linux:TARGET=../../../gears/Gear_Osc
mac:TARGET=../../../drone.app/Contents/Gears/Gear_Osc

