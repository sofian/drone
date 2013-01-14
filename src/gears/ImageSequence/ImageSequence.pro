include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ImageSequence.cpp
HEADERS+=Gear_ImageSequence.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_ImageSequence

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_ImageSequence
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT +=  xml qt3support 
