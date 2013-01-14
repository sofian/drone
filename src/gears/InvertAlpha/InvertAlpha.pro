include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_InvertAlpha.cpp
HEADERS+=Gear_InvertAlpha.h
INCLUDEPATH+=../../core/ ../../core/types ../../core/composite

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_InvertAlpha

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_InvertAlpha
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
