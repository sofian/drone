TEMPLATE=lib
SOURCES+=Gear_SignalStat.cpp
HEADERS+=Gear_SignalStat.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_SignalStat

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_SignalStat
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


