TEMPLATE=lib
SOURCES+=scanline0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/scanline0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/scanline0r
