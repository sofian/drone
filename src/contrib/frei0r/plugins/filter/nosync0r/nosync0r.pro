TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=nosync0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/nosync0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/nosync0r
