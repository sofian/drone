TEMPLATE=lib
SOURCES+=Gear_GrayScale.cpp
HEADERS+=Gear_GrayScale.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_GrayScale

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_GrayScale
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


