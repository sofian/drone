include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Ctrl_PushButton.cpp
HEADERS+=Ctrl_PushButton.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/control/Gear_PushButton

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/control/Ctrl_PushButton
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
