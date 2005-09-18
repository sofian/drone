include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Rescale.cpp
HEADERS+=Gear_Rescale.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Rescale

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Rescale
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


