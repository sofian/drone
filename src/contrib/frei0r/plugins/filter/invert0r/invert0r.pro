TEMPLATE=lib
SOURCES+=invert0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/invert0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/invert0r
