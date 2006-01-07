include (../../../../../config.pro)

TEMPLATE=lib
SOURCES+=Gear_Simple.cpp
HEADERS+=Gear_Simple.h
INCLUDEPATH+=../../../ ../../../types ../../../../contrib/frei0r/spec

#linux
LIBS+=-L../../../../../lib/ -ldroneCore

DESTDIR = ../../../../../testgears/drone



