include (../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AsciiArt.cpp
HEADERS+=Gear_AsciiArt.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore -laa 
#-L/usr/X11R6/lib -lX11 -lslang -lgpm
unix:!macx:TARGET=../../../gears/Gear_AsciiArt

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AsciiArt
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


