include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Enveloppe.cpp
HEADERS+=Gear_Enveloppe.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/Gear_Enveloppe

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_Enveloppe
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


