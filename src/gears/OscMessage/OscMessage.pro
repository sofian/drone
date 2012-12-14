include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscMessage.cpp
HEADERS+=Gear_OscMessage.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo


#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_OscMessage


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscMessage
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore



