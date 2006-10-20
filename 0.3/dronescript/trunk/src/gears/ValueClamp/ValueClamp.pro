include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ValueClamp.cpp
HEADERS+=Gear_ValueClamp.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_ValueClamp


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ValueClamp
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



