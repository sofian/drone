TEMPLATE=lib
SOURCES+=Gear_KDTree.cpp
HEADERS+=Gear_KDTree.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_KDTree

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_KDTree
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


