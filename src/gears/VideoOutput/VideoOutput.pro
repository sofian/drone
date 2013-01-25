include (../../../config.pro)

TEMPLATE=lib

CONFIG+=thread opengl

INCLUDEPATH+=../../core/ ../../core/types

#common
SOURCES+=Gear_VideoOutput.cpp DroneQGLWidget.cpp
HEADERS+=Gear_VideoOutput.h DroneQGLWidget.h DroneGLWindow.h

#linux
unix:!macx:LIBS+= -L../../../lib/ -ldroneCore -lGL
unix:!macx:TARGET=../../../gears/drone/Gear_VideoOutput

# osx
macx:INCLUDEPATH+= /Developer/Headers/FlatCarbon
macx:LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_VideoOutput

#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
