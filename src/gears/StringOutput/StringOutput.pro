include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringOutput.cpp GearGui_StringOutput.cpp
HEADERS+=Gear_StringOutput.h GearGui_StringOutput.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_StringOutput

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_StringOutput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
