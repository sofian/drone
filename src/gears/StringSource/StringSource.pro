include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringSource.cpp
HEADERS+=Gear_StringSource.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_StringSource


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_StringSource
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



