CONFIG += release

TEMPLATE = lib
TARGET = drone_frei0r_jni_Frei0r

INCLUDEPATH += ../contrib/Frei0r/spec

QMAKE_POST_LINK = mv lib$$TARGET* lib/

# Mac OS X specific
INCLUDEPATH += /System/Library/Frameworks/JavaVM.framework/Headers
QMAKE_LFLAGS += /System/Library/Frameworks/JavaVM.framework
CONFIG += lib_bundle
LIBS += -framework JavaVM
 
# Input
HEADERS += $$TARGET.h
SOURCES += $$TARGET.cpp
