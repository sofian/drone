include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_StringOutput.cpp GearGui_StringOutput.cpp
HEADERS+=Gear_StringOutput.h GearGui_StringOutput.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_StringOutput

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_StringOutput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
