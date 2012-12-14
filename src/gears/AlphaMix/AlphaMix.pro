include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AlphaMix.cpp
HEADERS+=Gear_AlphaMix.h
INCLUDEPATH+=../../core/ ../../core/types ../../core/composite

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AlphaMix

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AlphaMix
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
