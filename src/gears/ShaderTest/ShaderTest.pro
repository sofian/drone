include (../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_ShaderTest.cpp
HEADERS+=Gear_ShaderTest.h
INCLUDEPATH+=../../core/ ../../core/types

#linux
unix:!macx:LIBS+=-L../../../lib/ -ldroneCore 
unix:!macx:TARGET=../../../gears/Gear_ShaderTest

#osx
macx:TARGET=../../../drone.app/Contents/PlugIns/Gear_ShaderTest
macx:LIBS+=-L../../../drone.app/Contents/Frameworks -ldroneCore


#The following line was inserted by qt3to4
QT += xml core opengl qt3support
