include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_VideoInput.cpp
HEADERS+=Gear_VideoInput.h
INCLUDEPATH+=../../core/ ../../core/types
#INCLUDEPATH+=../../core/ ../../core/types /opt/local/include

QMAKE_CXXFLAGS+=-D__STDC_CONSTANT_MACROS

unix:!macx {
	CONFIG+=link_pkgconfig
#	PKGCONFIG+=libavformat libavcodec libswscale libavutil sdl
	PKGCONFIG += gstreamer-0.10

	QMAKE_LFLAGS_RPATH+=-Wl,-rpath=/opt/gstreamer-sdk/lib

	LIBS+=-L../../../lib/ -ldroneCore

	TARGET=../../../gears/drone/Gear_VideoInput
}

#osx
macx {
	INCLUDEPATH += /Library/Frameworks/GStreamer.framework/Headers
	LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L/opt/local/lib -lavformat -lavcodec -lavutil -lswscale -lz
	LIBS+=-framework GStreamer
	TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_VideoInput
	QMAKE_LFLAGS+=-read_only_relocs suppress
}

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
