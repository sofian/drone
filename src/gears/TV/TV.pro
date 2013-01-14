include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_TV.cpp GearGui_TV.cpp
HEADERS+=Gear_TV.h GearGui_TV.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_TV

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_TV
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
#The following line was inserted by qt3to4
QT += xml  qt3support 
