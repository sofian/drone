######################################################################
# Automatically generated by qmake (1.07a) Sat May 22 15:57:15 2004
######################################################################

include (config.pro)

linux:QMAKE_CXXFLAGS += -rdynamic
# Use gprof
gprof {
  QMAKE_CXXFLAGS+=-pg
}

# Debug settings
debug {
  OBJECTS_DIR = debug
  #QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=0
  QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=2 -DSINGLE_THREADED_PLAYBACK
}

# Optimized settings
release {
  OBJECTS_DIR = release
  QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=-1 -funroll-loops -fomit-frame-pointer -pipe
  p4 {
    QMAKE_CXXFLAGS += -march=pentium4
  }
  athlon {
    QMAKE_CXXFLAGS += -march=athlon    
  }

  ppc {
	QMAKE_CXXFLAGS += -O3 -faltivec
  }
  
}


TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += ../core ../core/types
linux:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,
linux:LIBS += -L../../lib -ldroneCore
mac:LIBS += -L../../drone.app/Contents/Frameworks -ldroneCore
TARGET = ../../drone

mac:LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
mac:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon

HEADERS += config.h \
GearListMenu.h \ 
GearPropertiesDialog.h \
MainWindow.h \
PropertyControl.h \
PropertyControlFilename.h \
PropertyControlString.h \
PropertyControlBool.h \
SchemaEditor.h \
Play.xpm \
Pause.xpm \
splash.xpm

SOURCES += drone.cpp \
GearListMenu.cpp \ 
GearPropertiesDialog.cpp \
MainWindow.cpp \
PropertyControl.cpp \
PropertyControlFilename.cpp \
PropertyControlString.cpp \
PropertyControlBool.cpp \
SchemaEditor.cpp
