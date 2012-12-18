include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_OscExtract.cpp
HEADERS+=Gear_OscExtract.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/liblo

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -L../../../lib/liblo/src/.libs -llo
unix:!macx:TARGET=../../../gears/Gear_OscExtract
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/liblo/src/.libs/,-rpath,


#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_OscExtract
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -llo



