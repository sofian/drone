include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Blur.cpp
HEADERS+=Gear_Blur.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Blur

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Blur
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
