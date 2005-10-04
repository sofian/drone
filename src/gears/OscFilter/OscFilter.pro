include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscFilter.cpp
HEADERS+=Gear_OscFilter.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -llo
unix:!macx:TARGET=../../../gears/Gear_OscFilter

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscFilter
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo



