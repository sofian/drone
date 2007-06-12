CONFIG += release

TEMPLATE = lib

TARGET = drone_artkp_kernel_TrackerMultiMarker

QMAKE_POST_LINK = mv lib$$TARGET* ../lib/

macx {
  INCLUDEPATH += /System/Library/Frameworks/JavaVM.framework/Headers
}

# Input
HEADERS += $$TARGET.h
SOURCES += $$TARGET.cpp
