TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=threshold0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/threshold0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/threshold0r
