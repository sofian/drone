######################################################################
# Automatically generated by qmake (1.07a) Sat May 22 15:57:15 2004
######################################################################

CONFIG = qt warn_on thread 
include (../../config.pro)

message(Drone : Builing Makefile with the following config flags : $$CONFIG)


unix:!macx:QMAKE_CXXFLAGS += -rdynamic

TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += ../core ../core/types ../contrib/frei0r/spec/

#mac
macx:LIBS += -L../../drone.app/Contents/Frameworks -ldroneCore -lguile
macx:LIBS += /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
macx:LIBS += /System/Library/Frameworks/Carbon.framework/Carbon
macx:TARGET = ../../drone.app/Contents/MacOS/drone
RC_FILE = myapp.icns
QMAKE_INFO_PLIST = Info.plist

#linux
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,
unix:!macx:LIBS += -L../../lib -ldroneCore -lguile
unix:!macx:TARGET = ../../drone


HEADERS += config.h \
GearListMenu.h \
GearListView.h \
GearPropertiesDialog.h \
GuileBindings.h \
MediaPoolIconView.h \
MediaPoolItem.h \
MainWindow.h \
MetaGearEditor.h \
MetaGearListMenu.h \
MediaPoolDialog.h \
#PreferencesDialog.h \
PanelScrollView.h \
PropertyControl.h \
PropertyControlFilename.h \
PropertyControlString.h \
PropertyControlBool.h \
PlugPropertiesTable.h \
PlugPropertiesTableItem.h \
PlugPropertiesTableItemValue.h \
PlugPropertiesTableItemString.h \
SchemaEditor.h \
Play.xpm \
Pause.xpm \
splash.xpm

SOURCES += drone.cpp \
GearListMenu.cpp \
GearListView.cpp \ 
GearPropertiesDialog.cpp \
GuileBindings.cpp \
MainWindow.cpp \
MediaPoolIconView.cpp \
MediaPoolItem.cpp \
MetaGearEditor.cpp \
MetaGearListMenu.cpp \
MediaPoolDialog.cpp \
PanelScrollView.cpp \
PropertyControl.cpp \
PropertyControlFilename.cpp \
PropertyControlString.cpp \
PropertyControlBool.cpp \
PlugPropertiesTable.cpp \
PlugPropertiesTableItem.cpp \
PlugPropertiesTableItemValue.cpp \
PlugPropertiesTableItemString.cpp \
#PreferencesDialog.cpp \
SchemaEditor.cpp

