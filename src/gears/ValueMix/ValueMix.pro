include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ValueMix.cpp
HEADERS+=Gear_ValueMix.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ValueMix

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ValueMix
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


