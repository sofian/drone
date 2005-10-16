include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaArrayClip.cpp
HEADERS+=Gear_AreaArrayClip.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaArrayClip

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaArrayClip
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


