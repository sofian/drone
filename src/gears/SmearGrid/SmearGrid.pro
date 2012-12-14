include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_SmearGrid.cpp
HEADERS+=Gear_SmearGrid.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_SmearGrid

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_SmearGrid
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
