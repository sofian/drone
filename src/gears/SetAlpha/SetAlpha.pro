include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_SetAlpha.cpp
HEADERS+=Gear_SetAlpha.h
INCLUDEPATH+=../../core/ ../../core/types ../../core/composite

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_SetAlpha

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_SetAlpha
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

