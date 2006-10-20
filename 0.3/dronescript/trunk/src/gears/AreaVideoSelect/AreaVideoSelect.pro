include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaVideoSelect.cpp
HEADERS+=Gear_AreaVideoSelect.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaVideoSelect

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaVideoSelect
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


