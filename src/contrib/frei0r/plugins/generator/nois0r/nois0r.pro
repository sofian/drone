TEMPLATE=lib
include (../../../../../../config.pro)

SOURCES+=nois0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/nois0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/nois0r
