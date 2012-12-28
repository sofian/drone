include (../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_VideoSource.cpp
HEADERS+=Gear_VideoSource.h

INCLUDEPATH+=../../core/ ../../core/types /opt/local/include

QMAKE_CXXFLAGS+=-D__STDC_CONSTANT_MACROS


unix:!macx {
#	system("/opt/gstreamer-sdk/bin/gst-sdk-shell")
	
	CONFIG+=link_pkgconfig
#	PKGCONFIG+=libavformat libavcodec libswscale libavutil sdl

	PKGCONFIG += gstreamer-0.10

	LIBS+=-L../../../lib/ -ldroneCore

	# -lavformat -lavcodec -ldl -lva -lXfixes -lXext -lX11 -ljack -lasound -lx264 -lvpx -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lrtmp -lgnutls -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfdk-aac -lfaac -lz -lrt -lswscale -lavutil -lm -lSDL
	TARGET=../../../gears/Gear_VideoSource
}

#osx
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L/opt/local/lib -lavformat -lavcodec -lavutil -lswscale -lz
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoSource
macx:QMAKE_LFLAGS+=-read_only_relocs suppress
#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
