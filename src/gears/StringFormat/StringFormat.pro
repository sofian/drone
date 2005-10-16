include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringFormat.cpp
HEADERS+=Gear_StringFormat.h
INCLUDEPATH+=../../core/ ../../core/types

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_StringFormat


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_StringFormat
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



