TEMPLATE=lib
SOURCES+=Gear_ApplyDisplaceGrid.cpp
HEADERS+=Gear_ApplyDisplaceGrid.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_ApplyDisplaceGrid

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_ApplyDisplaceGrid
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


