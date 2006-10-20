include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_PackList.cpp
HEADERS+=Gear_PackList.h
INCLUDEPATH+=../../core/ ../../core/types

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_PackList


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_PackList
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



