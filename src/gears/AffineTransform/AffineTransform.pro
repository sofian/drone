TEMPLATE=lib
SOURCES+=Gear_AffineTransform.cpp
HEADERS+=Gear_AffineTransform.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_AffineTransform

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_AffineTransform
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


