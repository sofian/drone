include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoOffset.cpp
HEADERS+=Gear_VideoOffset.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoOffset

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoOffset
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


