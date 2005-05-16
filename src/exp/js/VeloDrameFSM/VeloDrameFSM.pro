include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VeloDrameFSM.cpp
HEADERS+=Gear_VeloDrameFSM.h
INCLUDEPATH+=../../../core/ ../../../core/types

#linux
unix:!macx:LIBS+=-L../../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../../gears/Gear_VeloDrameFSM

#osx
macx:TARGET=../../../../drone.app/Contents/PlugIns/Gear_VeloDrameFSM
macx:LIBS+=-L../../../../drone.app/Contents/Frameworks -ldroneCore


