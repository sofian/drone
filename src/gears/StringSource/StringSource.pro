include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringSource.cpp
HEADERS+=Gear_StringSource.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo


#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_StringSource


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_StringSource
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
