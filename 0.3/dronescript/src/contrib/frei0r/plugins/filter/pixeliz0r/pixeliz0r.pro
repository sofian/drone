TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=pixeliz0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/pixeliz0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/pixeliz0r
