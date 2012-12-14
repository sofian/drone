include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Osc.cpp
HEADERS+=Gear_Osc.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Osc

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Osc
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

#The following line was inserted by qt3to4
QT += xml  qt3support 
