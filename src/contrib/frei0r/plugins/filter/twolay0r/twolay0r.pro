TEMPLATE=lib
SOURCES+=twolay0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/twolay0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/twolay0r
