include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoMix.cpp
HEADERS+=Gear_VideoMix.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoMix

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoMix
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


