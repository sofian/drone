include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoDelay.cpp
HEADERS+=Gear_VideoDelay.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_VideoDelay

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_VideoDelay
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
