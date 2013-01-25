include (../../../config.pro)

CONFIG += thread

TEMPLATE=lib
SOURCES+=Gear_AudioOutput.cpp
HEADERS+=Gear_AudioOutput.h
INCLUDEPATH+=../../core/ ../../core/types

unix:!macx {
	CONFIG+=link_pkgconfig
	PKGCONFIG += gstreamer-0.10 gstreamer-base-0.10 gstreamer-app-0.10

	QMAKE_LFLAGS_RPATH+=-Wl,-rpath=/opt/gstreamer-sdk/lib

	LIBS+=-L../../../lib/ -ldroneCore

	TARGET=../../../gears/drone/Gear_AudioOutput
}

#osx
macx {
	INCLUDEPATH += /Library/Frameworks/GStreamer.framework/Headers
#	LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore -L/opt/local/lib -lavformat -lavcodec -lavutil -lswscale -lz
	LIBS+=-framework GStreamer
	TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_VideoSource
	QMAKE_LFLAGS+=-read_only_relocs suppress
}


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
