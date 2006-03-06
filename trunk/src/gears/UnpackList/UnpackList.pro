include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_UnpackList.cpp
HEADERS+=Gear_UnpackList.h
INCLUDEPATH+=../../core/ ../../core/types

macx:QMAKE_CXXFLAGS += -Wno-long-double

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_UnpackList


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_UnpackList
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



