TEMPLATE=lib
SOURCES+=onecol0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/onecol0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/onecol0r
