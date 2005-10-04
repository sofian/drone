include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscInput.cpp
HEADERS+=Gear_OscInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -llo
unix:!macx:TARGET=../../../gears/Gear_OscInput

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscInput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo



