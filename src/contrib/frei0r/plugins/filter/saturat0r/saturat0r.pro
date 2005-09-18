TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=saturat0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/saturat0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/saturat0r
