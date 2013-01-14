include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_GenDisplaceGrid.cpp
HEADERS+=Gear_GenDisplaceGrid.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_GenDisplaceGrid

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_GenDisplaceGrid
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
