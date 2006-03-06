TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=gamma.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/gamma

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/gamma
