TEMPLATE=lib
SOURCES+=Gear_VideoDelay.cpp
HEADERS+=Gear_VideoDelay.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoDelay

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoDelay
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


