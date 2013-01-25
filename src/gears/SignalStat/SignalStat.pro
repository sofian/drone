include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_SignalStat.cpp
HEADERS+=Gear_SignalStat.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_SignalStat

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_SignalStat
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
