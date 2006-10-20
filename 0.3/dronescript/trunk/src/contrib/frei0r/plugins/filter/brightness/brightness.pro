TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=brightness.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/brightness

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/brightness
