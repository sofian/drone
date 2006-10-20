TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=bw0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/bw0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/bw0r
