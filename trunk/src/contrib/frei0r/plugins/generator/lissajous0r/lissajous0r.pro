TEMPLATE=lib
SOURCES+=lissajous0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/lissajous0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/lissajous0r
