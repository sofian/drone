include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ColorQuantize.cpp
HEADERS+=Gear_ColorQuantize.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ColorQuantize

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ColorQuantize
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


