TEMPLATE=lib
SOURCES+=Gear_AreaArrayMask.cpp
HEADERS+=Gear_AreaArrayMask.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaArrayMask

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaArrayMask
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


