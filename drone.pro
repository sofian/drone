TEMPLATE = subdirs
system( ./buildlibs )
macx:system( ./bundlelibs.osx )
SUBDIRS = src/core src/gears src/drone src/exp/piksel05
#src/contrib


