include (../config.pro)

TEMPLATE=lib

SOURCES+=Gear_VideoSource.cpp
HEADERS+=Gear_VideoSource.h

INCLUDEPATH+=../../core/ ../../core/types ../../../lib/ffmpeg/libavcodec ../../../lib/ffmpeg/libavformat
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -ldroneCore -lavformat -lavcodec
unix:!macx:TARGET=../../../gears/Gear_VideoSource										    	

#osx
#call ranlib on prelinked static libs on mac
macx:system( ranlib ../../../lib/ffmpeg/libavformat/libavformatmac.a )
macx:system( ranlib ../../../lib/ffmpeg/libavcodec/libavcodecmac.a )
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -lavformat -lavcodec -lz
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoSource
macx:QMAKE_LFLAGS+=-read_only_relocs suppress
