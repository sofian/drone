TEMPLATE=lib
SOURCES+=flippo.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/flippo

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/flippo
