TEMPLATE=lib
SOURCES+=Gear_AudioInput.cpp
HEADERS+=Gear_AudioInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/portaudio/include
QMAKE_RPATH = -Wl,-rpath,lib/portaudio,-rpath,

#linux
linux:LIBS+=-L../../../lib/ -L../../../lib/portaudio -ldroneCore -lportaudio
linux:TARGET=../../../gears/Gear_AudioInput

#osx
#todo add portaudio
mac:TARGET=../../../drone.app/Contents/PlugIns/Gear_AudioInput
mac:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


