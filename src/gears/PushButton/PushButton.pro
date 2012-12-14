include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_PushButton.cpp GearGui_PushButton.cpp
HEADERS+=Gear_PushButton.h GearGui_PushButton.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_PushButton

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_PushButton
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
