include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoInput.cpp
HEADERS+=Gear_VideoInput.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoInput

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoInput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


