include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_HalfToning.cpp
HEADERS+=Gear_HalfToning.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_HalfToning

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_HalfToning
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


