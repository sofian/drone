include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ListBox.cpp GearGui_ListBox.cpp
HEADERS+=Gear_ListBox.h GearGui_ListBox.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ListBox

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ListBox
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


