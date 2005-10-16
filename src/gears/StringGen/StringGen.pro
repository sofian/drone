include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringGen.cpp
HEADERS+=Gear_StringGen.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_StringGen


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_StringGen
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



