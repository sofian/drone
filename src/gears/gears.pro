include(config.pro)

TEMPLATE=subdirs
SUBDIRS= VideoOutput \
VideoSource \
Slider

agg {
SUBDIRS+=AggTest
}

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

