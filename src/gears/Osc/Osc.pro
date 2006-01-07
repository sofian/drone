include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Osc.cpp
HEADERS+=Gear_Osc.h
INCLUDEPATH+=../../core/ ../../core/types

LIBS+=-L../../../lib/ -ldroneCore

#linux
unix:!macx:DESTDIR=../../../gears/Gear_Osc

#osx
macx:DESTDIR=../../../drone.app/Contents/PlugIns/Gear_Osc

