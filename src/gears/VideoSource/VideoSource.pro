include (../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_VideoSource.cpp
HEADERS+=Gear_VideoSource.h

INCLUDEPATH+=../../core/ ../../core/types /opt/local/include

unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/ffmpeg/libavcodec -L../../../lib/ffmpeg/libavformat -ldroneCore -lavformat -lavcodec
unix:!macx:TARGET=../../../gears/drone/Gear_VideoSource										    	

#osx
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L/opt/local/lib -lavformat -lavcodec -lavutil -lswscale -lz
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_VideoSource
macx:QMAKE_LFLAGS+=-read_only_relocs suppress
#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
