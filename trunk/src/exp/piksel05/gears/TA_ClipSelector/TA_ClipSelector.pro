include (../../../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_TA_ClipSelector.cpp
HEADERS+=Gear_TA_ClipSelector.h


INCLUDEPATH+=../../../../core/ ../../core/ ../../../../core/types
unix:!macx:LIBS+=-L../../../../../lib/ -ldroneCore -ldroneTACore
unix:!macx:TARGET=../../../../../gears/Gear_TA_ClipSelector
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,
        
#osx
macx:LIBS+=-L../../../../../drone.app/Contents/Frameworks -ldroneCore -ldroneTACore -L../../../../../lib/
macx:TARGET=../../../../../drone.app/Contents/PlugIns/Gear_TA_ClipSelector
macx:QMAKE_LFLAGS+=-read_only_relocs suppress

