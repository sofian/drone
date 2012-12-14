include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoSwitch.cpp
HEADERS+=Gear_VideoSwitch.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_VideoSwitch

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoSwitch
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
