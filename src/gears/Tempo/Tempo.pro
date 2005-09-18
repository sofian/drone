include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Tempo.cpp
HEADERS+=Gear_Tempo.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Tempo

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Tempo
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
