include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ImageSource.cpp
HEADERS+=Gear_ImageSource.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ImageSource

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ImageSource
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


