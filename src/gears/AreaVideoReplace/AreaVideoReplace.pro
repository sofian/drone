include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AreaVideoReplace.cpp
HEADERS+=Gear_AreaVideoReplace.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_AreaVideoReplace

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AreaVideoReplace
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


