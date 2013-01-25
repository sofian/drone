CONFIG = qt warn_on thread debug resources

QT += qt3support xml 

message(Gears : Builing Makefile with the following config flags : $$CONFIG)

macx:QMAKE_CXXFLAGS += -fpermissive  


# Debug settings
debug {
  OBJECTS_DIR = debug
  #QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=0
  QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=2 -DSINGLE_THREADED_PLAYBACK -O0
}

# Optimized settings
release {
  OBJECTS_DIR = release  
  QMAKE_CXXFLAGS_RELEASE -= -O2 -Os
  
  unix:!macx:QMAKE_CXXFLAGS += -DDEBUG_LEVEL=-1 -funroll-loops -fomit-frame-pointer -pipe -O3
  unix:!macx:QMAKE_CFLAGS += -DDEBUG_LEVEL=-1 -funroll-loops -fomit-frame-pointer -pipe -O3

  macx:QMAKE_CXXFLAGS += -DDEBUG_LEVEL=-1 -O1 -ffast-math -funroll-loops -frename-registers -mcpu=G4 -mtune=G4
  macx:QMAKE_CFLAGS += -DDEBUG_LEVEL=-1 -O1 -ffast-math -funroll-loops -frename-registers -mcpu=G4 -mtune=G4

  
  p4 {
    QMAKE_CXXFLAGS += -march=pentium4
    QMAKE_CFLAGS += -march=pentium4
  }
  p3 {
    QMAKE_CXXFLAGS += -march=pentium3
    QMAKE_CFLAGS += -march=pentium3
  }
  athlon {
    QMAKE_CXXFLAGS += -march=athlon    
    QMAKE_CFLAGS += -march=athlon
  }
  athlonxp {
    QMAKE_CXXFLAGS += -march=athlon-xp
    QMAKE_CFLAGS += -march=athlon-xp

  }

}

INCLUDEPATH += ../../contrib/frei0r/spec
#disable prebinding
macx:QMAKE_LFLAGS_SHLIB -= -prebind

agg {
  QMAKE_CXXFLAGS += -I../../../lib/agg23/include
}
