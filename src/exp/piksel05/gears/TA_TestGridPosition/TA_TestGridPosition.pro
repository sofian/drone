include (../../../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_TA_TestGridPosition.cpp
HEADERS+=Gear_TA_TestGridPosition.h

INCLUDEPATH+=../../../../core/ ../../core/ ../../../../core/types
unix:!macx:LIBS+=-L../../../../../lib/ -ldroneCore -ldroneTACore
unix:!macx:TARGET=../../../../../gears/Gear_TA_TestGridPosition
unix:!macx:QMAKE_RPATH = -Wl,-rpath,src/exp/piksel05/lib/,-rpath

#osx
macx:LIBS+=-L../../../../../drone.app/Contents/Frameworks -ldroneCore -ldroneTACore
macx:TARGET=../../../../../drone.app/Contents/PlugIns/Gear_TA_TestGridPosition
macx:QMAKE_LFLAGS+=-read_only_relocs suppress
