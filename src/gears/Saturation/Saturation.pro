include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Saturation.cpp
HEADERS+=Gear_Saturation.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Saturation

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Saturation
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


