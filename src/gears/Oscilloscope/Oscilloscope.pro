include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Oscilloscope.cpp
HEADERS+=Gear_Oscilloscope.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Oscilloscope

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Oscilloscope
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

