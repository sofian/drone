CONFIG = qt warn_on thread 

include(../../flags.pro)

message(Gears : Builing Makefile with the following config flags : $$CONFIG)


# Debug settings
debug {
  OBJECTS_DIR = debug
  #QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=0
  QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=2 -DSINGLE_THREADED_PLAYBACK
}

# Optimized settings
release {
  OBJECTS_DIR = release  
  QMAKE_CXXFLAGS_RELEASE -= -O2 -Os
  
  unix:!macx:QMAKE_CXXFLAGS += -DDEBUG_LEVEL=-1 -funroll-loops -fomit-frame-pointer -pipe -O3
  macx:QMAKE_CXXFLAGS += -DDEBUG_LEVEL=-1 -funroll-loops -mcpu=G4 -mtune=G4 -Os  
  
  p4 {
    QMAKE_CXXFLAGS += -march=pentium4
  }
  p3 {
    QMAKE_CXXFLAGS += -march=pentium3
  }
  athlon {
    QMAKE_CXXFLAGS += -march=athlon    
  }

  G5 {
	QMAKE_CXXFLAGS += -DDEBUG_LEVEL=-1 -fast
  }  
}

agg {
  QMAKE_CXXFLAGS += -I../../agg2/include
}
