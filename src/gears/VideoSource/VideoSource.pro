TEMPLATE=lib

unix:!macx:SOURCES+=Gear_VideoSource.cpp
unix:!macx:HEADERS+=Gear_VideoSource.h

unix:!macx:INCLUDEPATH+=../../core/ ../../core/types ../../../lib/linux/libmpeg3/include
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/linux/libmpeg3/ -ldroneCore -lmpeg3
unix:!macx:TARGET=../../../gears/Gear_VideoSource

#osx
macx:SOURCES+=Gear_VideoSourceOSX.cpp
macx:HEADERS+=Gear_VideoSourceOSX.h

macx:INCLUDEPATH+=../../core/ ../../core/types
macx:INCLUDEPATH+= /Developer/Headers/FlatCarbon
macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
macx:LIBS += -framework Quicktime
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoSource

