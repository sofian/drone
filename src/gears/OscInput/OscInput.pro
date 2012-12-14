include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscInput.cpp
HEADERS+=Gear_OscInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/liblo/src/.libs -llo
unix:!macx:TARGET=../../../gears/Gear_OscInput
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/liblo/src/.libs/,-rpath,


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscInput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo



