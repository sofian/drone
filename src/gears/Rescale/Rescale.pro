include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Rescale.cpp
HEADERS+=Gear_Rescale.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_Rescale

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_Rescale
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
