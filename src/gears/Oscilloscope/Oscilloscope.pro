TEMPLATE=lib
SOURCES+=Gear_Oscilloscope.cpp
HEADERS+=Gear_Oscilloscope.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_Oscilloscope

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Oscilloscope
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

