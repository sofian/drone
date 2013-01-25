include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Gamma.cpp
HEADERS+=Gear_Gamma.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_Gamma

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_Gamma
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
