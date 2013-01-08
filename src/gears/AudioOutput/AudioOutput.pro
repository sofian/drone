include (../../../config.pro)

CONFIG += thread

TEMPLATE=lib
SOURCES+=Gear_AudioOutput.cpp
HEADERS+=Gear_AudioOutput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/portaudio/include
QMAKE_LFLAGS_RPATH = -Wl,-rpath,lib/portaudio,-rpath,

#linux
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/portaudio -ldroneCore -lportaudio
unix:!macx:TARGET=../../../gears/Gear_AudioOutput

#osx
#todo add portaudio
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_AudioOutput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
