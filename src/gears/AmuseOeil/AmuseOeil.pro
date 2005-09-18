include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AmuseOeil.cpp
HEADERS+=Gear_AmuseOeil.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../agg2/src/ -lagg
unix:!macx:TARGET=../../../gears/Gear_AmuseOeil

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AmuseOeil
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L../../agg2/src/ -lagg


