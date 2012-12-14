include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoMix.cpp
HEADERS+=Gear_VideoMix.h
INCLUDEPATH+=../../core/ ../../core/types ../../core/composite

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoMix

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoMix
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
