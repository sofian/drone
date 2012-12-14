include (../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_SelectFrame.cpp
HEADERS+=Gear_SelectFrame.h

INCLUDEPATH+=../../core/ ../../core/types ../../../lib/ffmpeg/libavcodec ../../../lib/ffmpeg/libavformat
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -ldroneCore -lavformat -lavcodec
unix:!macx:TARGET=../../../gears/Gear_SelectFrame										    	

#osx
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -lavformat -lavcodec -lz
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_SelectFrame
macx:QMAKE_LFLAGS+=-read_only_relocs suppress
#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
