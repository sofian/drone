include (../../../../config.pro)

TEMPLATE=lib
SOURCES+=Ctrl_PushButton.cpp
HEADERS+=Ctrl_PushButton.h
INCLUDEPATH+=../../../core/ ../../../drone ../../../core/types

#linux
unix:!macx:LIBS+=-L../../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../../gears/controls/Ctrl_PushButton

#osx
macx:TARGET=../../../../drone.app/Contents/PlugIns/controls/Ctrl_PushButton
macx:LIBS+=-L../../../../drone.app/Contents/Frameworks -ldroneCore -L../../../../drone.app/Contents/PlugIns/drone  


#The following line was inserted by qt3to4
QT += xml  qt3support 
