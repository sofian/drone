include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoReplaceAreas.cpp
HEADERS+=Gear_VideoReplaceAreas.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoReplaceAreas

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoReplaceAreas
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


