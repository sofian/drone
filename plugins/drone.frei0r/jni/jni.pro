CONFIG += release

TEMPLATE = lib
TARGET = drone_frei0r_jni_Frei0r

INCLUDEPATH += ../contrib/Frei0r/spec

QMAKE_POST_LINK = mv lib$$TARGET* lib/

macx {
  INCLUDEPATH += /System/Library/Frameworks/JavaVM.framework/Headers
}

# Input
HEADERS += $$TARGET.h
SOURCES += $$TARGET.cpp
