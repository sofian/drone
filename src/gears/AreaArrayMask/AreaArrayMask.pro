include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaArrayMask.cpp
HEADERS+=Gear_AreaArrayMask.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_AreaArrayMask

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_AreaArrayMask
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


