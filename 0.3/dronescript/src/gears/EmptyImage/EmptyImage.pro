include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_EmptyImage.cpp
HEADERS+=Gear_EmptyImage.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_EmptyImage

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_EmptyImage
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


