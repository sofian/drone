TEMPLATE=lib
SOURCES+=Gear_KDTree.cpp
HEADERS+=Gear_KDTree.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_KDTree

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_KDTree
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


