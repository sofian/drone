TEMPLATE=lib
SOURCES+=delay0r.cpp
INCLUDEPATH+=../../../spec/ ../../common/

#linux
unix:!macx:TARGET=../../../../../../gears/frei0r/delay0r

#osx
macx:TARGET=../../../../../../drone.app/Contents/PlugIns/frei0r/delay0r
