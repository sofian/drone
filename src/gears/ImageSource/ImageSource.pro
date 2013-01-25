include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ImageSource.cpp
HEADERS+=Gear_ImageSource.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_ImageSource

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/drone/Gear_ImageSource
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT +=  xml qt3support 
