TEMPLATE=lib
SOURCES+=Gear_TouchPad.cpp GearGui_TouchPad.cpp
HEADERS+=Gear_TouchPad.h GearGui_TouchPad.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_TouchPad

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_TouchPad
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


