include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ColorAdjust.cpp
HEADERS+=Gear_ColorAdjust.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ColorAdjust

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ColorAdjust
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


