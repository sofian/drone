TEMPLATE=lib
SOURCES+=Gear_PushButton.cpp GearGui_PushButton.cpp
HEADERS+=Gear_PushButton.h GearGui_PushButton.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_PushButton

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_PushButton
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


