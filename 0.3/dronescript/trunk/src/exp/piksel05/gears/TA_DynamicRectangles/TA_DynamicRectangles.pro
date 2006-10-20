include (../../../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_TA_DynamicRectangles.cpp
HEADERS+=Gear_TA_DynamicRectangles.h


INCLUDEPATH+=../../../../core/ ../../core/ ../../../../core/types ../../../../core/composite
unix:!macx:LIBS+=-L../../../../../lib/ -ldroneCore -ldroneTACore
unix:!macx:TARGET=../../../../../gears/Gear_TA_DynamicRectangles										  
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,
        
#osx
macx:LIBS+=-L../../../../../drone.app/Contents/Frameworks -ldroneCore -ldroneTACore -L../../../../../lib/
macx:TARGET=../../../../../drone.app/Contents/PlugIns/Gear_TA_DynamicRectangles
macx:QMAKE_LFLAGS+=-read_only_relocs suppress

