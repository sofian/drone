include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_TouchPad.cpp GearGui_TouchPad.cpp
HEADERS+=Gear_TouchPad.h GearGui_TouchPad.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_TouchPad

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_TouchPad
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
