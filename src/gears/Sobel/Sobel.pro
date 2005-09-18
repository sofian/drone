include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Sobel.cpp
HEADERS+=Gear_Sobel.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Sobel

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Sobel
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


