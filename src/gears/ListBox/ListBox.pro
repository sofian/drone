TEMPLATE=lib
SOURCES+=Gear_ListBox.cpp GearGui_ListBox.cpp
HEADERS+=Gear_ListBox.h GearGui_ListBox.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_ListBox

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_ListBox
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


