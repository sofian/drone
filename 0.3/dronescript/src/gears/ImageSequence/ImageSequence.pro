include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ImageSequence.cpp
HEADERS+=Gear_ImageSequence.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ImageSequence

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ImageSequence
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


