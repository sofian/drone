TEMPLATE=lib
SOURCES+=brightness.c
INCLUDEPATH+=../../

#linux
unix:!macx:TARGET=../../../../../gears/frei0r/brightness

#osx
macx:TARGET=../../../../../drone.app/Contents/PlugIns/frei0r/brightness

