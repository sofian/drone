include (../../../config.pro)

TEMPLATE=lib

CONFIG+=thread opengl

#common
SOURCES+=Gear_VideoOutput.cpp DroneQGLWidget.cpp
HEADERS+=Gear_VideoOutput.h DroneQGLWidget.h DroneGLWindow.h

INCLUDEPATH+=../../core/ ../../core/types

unix:!macx {
LIBS+= -L../../../lib/ -ldroneCore -lGL
TARGET=../../../gears/Gear_VideoOutput
}

macx {
INCLUDEPATH+= /Developer/Headers/FlatCarbon
LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
TARGET=../../../drone.app/Contents/PlugIns/Gear_VideoOutput
}

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
