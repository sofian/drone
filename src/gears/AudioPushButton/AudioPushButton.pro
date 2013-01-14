include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_AudioPushButton.cpp GearGui_AudioPushButton.cpp
HEADERS+=Gear_AudioPushButton.h GearGui_AudioPushButton.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore
unix:!macx:TARGET=../../../gears/drone/Gear_AudioPushButton

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_AudioPushButton
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


