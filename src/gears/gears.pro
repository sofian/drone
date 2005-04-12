include(config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
AmuseOeil \
TV \
Oscilloscope \
VideoOutput \
AlphaMix \
Blur \
VideoSource \
Slider \
KDTree\ 
VideoMix \
Edge \
AffineTransform \
ApplyDisplaceGrid \
AreaArrayMask \
#BeatDetector \
Brightness \
ClusteredDither \
ColorAdjust \
Contrast \
Enveloppe \
FlatSignal \
ListBox \
PushButton \
Rescale \
TouchPad \
TapTempo \
ColorQuantize \
Gamma \
GenDisplaceGrid \
GrayScale \
HalfToning \
Saturation \
SetAlpha \
SignalStat \
SmearGrid \
Sobel \
#VideoDelay \
#VideoLoop \
ImageShuffle \
ImageSequence \
Invert \
InvertAlpha \
VideoRecord \
VideoOffset \
ValueMix
agg {
SUBDIRS+=AggTest
}

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

