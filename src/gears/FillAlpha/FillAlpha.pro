include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_FillAlpha.cpp
HEADERS+=Gear_FillAlpha.h
INCLUDEPATH+=../../core/ ../../core/types ../../core/composite

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_FillAlpha

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_FillAlpha
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
