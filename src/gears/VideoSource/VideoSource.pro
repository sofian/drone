include (../config.pro)

TEMPLATE=lib

SOURCES+=Gear_VideoSource.cpp
HEADERS+=Gear_VideoSource.h

INCLUDEPATH+=../../core/ ../../core/types ../../../lib/ffmpeg/libavcodec ../../../lib/ffmpeg/libavformat
LIBS+=-L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -ldroneCore -lavformat -lavcodec
unix:!macx:TARGET=../../../gears/Gear_VideoSource										    	

#osx
#macx:SOURCES+=Gear_VideoSourceOSX.cpp
#macx:HEADERS+=Gear_VideoSourceOSX.h
#macx:INCLUDEPATH+=../../core/ ../../core/types
#macx:INCLUDEPATH+= /Developer/Headers/FlatCarbon
#macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
#macx:LIBS += -framework Quicktime
#macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoSource

