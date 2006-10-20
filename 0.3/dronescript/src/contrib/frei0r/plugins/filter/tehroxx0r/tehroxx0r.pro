TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=tehroxx0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/tehroxx0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/tehroxx0r
