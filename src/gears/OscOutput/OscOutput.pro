include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscOutput.cpp
HEADERS+=Gear_OscOutput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo


#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/liblo/src/.libs -llo
unix:!macx:TARGET=../../../gears/drone/Gear_OscOutput
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/liblo/src/.libs/,-rpath,


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_OscOutput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo



