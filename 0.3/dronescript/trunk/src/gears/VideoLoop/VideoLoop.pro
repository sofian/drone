include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoLoop.cpp
HEADERS+=Gear_VideoLoop.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoLoop

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoLoop
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


