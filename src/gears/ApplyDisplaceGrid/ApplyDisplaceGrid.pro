#include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ApplyDisplaceGrid.cpp
HEADERS+=Gear_ApplyDisplaceGrid.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ApplyDisplaceGrid

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ApplyDisplaceGrid
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


