TEMPLATE=lib
SOURCES+=gamma.c
INCLUDEPATH+=../../

#linux
unix:!macx:TARGET=../../../../../gears/frei0r/gamma

#osx
macx:TARGET=../../../../../drone.app/Contents/PlugIns/frei0r/gamma

