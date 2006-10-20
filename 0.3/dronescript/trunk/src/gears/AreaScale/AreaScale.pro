include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaScale.cpp
HEADERS+=Gear_AreaScale.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaScale

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaScale
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


