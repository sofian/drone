include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ColorAdjust.cpp
HEADERS+=Gear_ColorAdjust.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_ColorAdjust

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_ColorAdjust
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
