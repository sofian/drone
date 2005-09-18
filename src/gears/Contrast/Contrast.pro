include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Contrast.cpp
HEADERS+=Gear_Contrast.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Contrast

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Contrast
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


