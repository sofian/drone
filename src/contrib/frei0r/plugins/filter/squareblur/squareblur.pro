TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=squareblur.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/squareblur

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/squareblur
