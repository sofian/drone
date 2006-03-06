TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=distort0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/distort0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/distort0r
