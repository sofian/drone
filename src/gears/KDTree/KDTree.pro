include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_KDTree.cpp
HEADERS+=Gear_KDTree.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_KDTree

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_KDTree
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
