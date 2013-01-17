include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_PushButton.cpp
HEADERS+=Gear_PushButton.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_PushButton

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Ctrl_PushButton
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
