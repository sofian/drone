TEMPLATE=lib
SOURCES+=Gear_Slider.cpp GearGui_Slider.cpp
HEADERS+=Gear_Slider.h GearGui_Slider.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:LIBS+=-L../../../lib/ -ldroneCore
unix:TARGET=../../../gears/Gear_Slider

#osx
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_Slider
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


