include (../../../config.pro)

CONFIG += thread

TEMPLATE=lib
SOURCES+=Gear_AudioInput.cpp
HEADERS+=Gear_AudioInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/portaudio/include
QMAKE_LFLAGS_RPATH = -Wl,-rpath,lib/portaudio,-rpath,

#linux
unix:!macx:LIBS+=-L../../../lib/ -L../../../lib/portaudio -ldroneCore -lportaudio
unix:!macx:TARGET=../../../gears/drone/Gear_AudioInput

#osx
#todo add portaudio
macx:TARGET=../../../drone.app/Contents/PlugIns/drone/Gear_AudioInput
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
