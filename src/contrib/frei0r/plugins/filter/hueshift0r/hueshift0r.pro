TEMPLATE=lib
SOURCES+=hueshift0r.c
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/hueshift0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/hueshift0r
