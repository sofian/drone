include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ImageShuffle.cpp
HEADERS+=Gear_ImageShuffle.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ImageShuffle

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ImageShuffle
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


