TEMPLATE=lib
SOURCES+=Gear_AudioInput.cpp
HEADERS+=Gear_AudioInput.h
INCLUDEPATH+=../../core/ ../../core/types ../../../lib/portaudio/include
LIBS+=-L../../../lib/ -L../../../lib/portaudio -ldroneCore -lportaudio
QMAKE_RPATH = -Wl,-rpath,lib/portaudio,-rpath,
TARGET=../../../gears/Gear_AudioInput


