TEMPLATE=lib
SOURCES+=Gear_Brightness.cpp
HEADERS+=Gear_Brightness.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Brightness

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Brightness
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


