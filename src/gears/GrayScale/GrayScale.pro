include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_GrayScale.cpp
HEADERS+=Gear_GrayScale.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_GrayScale

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_GrayScale
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
macx:QMAKE_CXXFLAGS += -D__MACOSX_CORE__

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
