include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AggTest.cpp
HEADERS+=Gear_AggTest.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/agg23/src/ -lagg
unix:!macx:TARGET=../../../gears/drone/Gear_AggTest

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_AggTest
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


