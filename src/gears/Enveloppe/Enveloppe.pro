TEMPLATE=lib
SOURCES+=Gear_Enveloppe.cpp
HEADERS+=Gear_Enveloppe.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
linux:LIBS+=-L../../../lib/ -ldroneCore
linux:TARGET=../../../gears/Gear_Enveloppe

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Enveloppe
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore

