include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_BeatDetector.cpp
HEADERS+=Gear_BeatDetector.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_BeatDetector

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_BeatDetector
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


