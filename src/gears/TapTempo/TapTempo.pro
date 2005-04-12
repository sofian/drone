include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_TapTempo.cpp
HEADERS+=Gear_TapTempo.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_TapTempo

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_TapTempo
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


