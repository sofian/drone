include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaArrayVideoSelect.cpp
HEADERS+=Gear_AreaArrayVideoSelect.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaArrayVideoSelect

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaArrayVideoSelect
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


