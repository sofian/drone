include(config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
TV \
Oscilloscope \
VideoOutput \
AudioPushButton \
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
FillAlpha \
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
#ImageShuffle \
ImageSource \
Invert \
InvertAlpha \
Saturation \
SelectFrame \
SetAlpha \
SignalStat \
SmearGrid \
Sobel \
Tempo \
#VideoDelay \
#VideoLoop \
#ImageShuffle \
#ImageSequence \
VideoRecord \
VideoOffset \
ValueMix
agg {
SUBDIRS+=AggTest \
AmuseOeil
}

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

