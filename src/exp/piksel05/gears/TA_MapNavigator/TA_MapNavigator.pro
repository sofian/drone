include (../../../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_TA_MapNavigator.cpp
HEADERS+=Gear_TA_MapNavigator.h


INCLUDEPATH+=../../../../core/ ../../core/ ../../../../core/types
unix:!macx:LIBS+=-L../../../../../lib/ -L../../lib -ldroneCore -ldroneTACore
unix:!macx:TARGET=../../../../../gears/Gear_TA_MapNavigator										  
QMAKE_RPATH = -Wl,-rpath,src/exp/piksel05/lib/,-rpath,

#osx
macx:LIBS+=-L../../../../../drone.app/Contents/Frameworks -ldroneCore -ldroneTACore -L../../../../../lib/ -L../../lib
macx:TARGET=../../../../../drone.app/Contents/PlugIns/Gear_TA_MapNavigator
macx:QMAKE_LFLAGS+=-read_only_relocs suppress

