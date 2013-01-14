include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AmuseOeil.cpp
HEADERS+=Gear_AmuseOeil.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/agg23/src/ -lagg
unix:!macx:TARGET=../../../gears/drone/Gear_AmuseOeil

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_AmuseOeil
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L../../../lib/agg23/src/ -lagg


