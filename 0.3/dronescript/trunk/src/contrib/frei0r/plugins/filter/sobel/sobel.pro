TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=sobel.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/sobel

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/sobel
