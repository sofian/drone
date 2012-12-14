include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_RandomValue.cpp
HEADERS+=Gear_RandomValue.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo


#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_RandomValue


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_RandomValue
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
