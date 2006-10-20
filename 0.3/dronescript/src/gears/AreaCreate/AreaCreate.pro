include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaCreate.cpp
HEADERS+=Gear_AreaCreate.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaCreate

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaCreate
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


