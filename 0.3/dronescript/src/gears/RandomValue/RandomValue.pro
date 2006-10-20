include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_RandomValue.cpp
HEADERS+=Gear_RandomValue.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_RandomValue


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_RandomValue
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



