include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Frei0r.cpp
HEADERS+=Gear_Frei0r.h
INCLUDEPATH+=../../core/ ../../core/types ../../contrib/frei0r

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Frei0r

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Frei0r
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


