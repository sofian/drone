TEMPLATE=lib
SOURCES+=ising0r.c
INCLUDEPATH+=../../../spec/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/ising0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/ising0r
