TEMPLATE=subdirs
SUBDIRS= Osc \
TV \
Oscilloscope \
VideoOutput \
Blur \
VideoSource \
Slider \
KDTree\ 
VideoMix \
Edge \
AffineTransform \
ApplyDisplaceGrid \
AreaArrayMask \
Brightness \
ClusteredDither \
ColorAdjust \
Contrast \
Enveloppe \
FlatSignal \
ListBox \
PushButton \
TouchPad \
ColorQuantize \
Gamma \
GenDisplaceGrid \
GrayScale \
HalfToning \
Saturation \
SignalStat \
SmearGrid \
VideoDelay \
VideoLoop \
ImageShuffle \
ImageSequence \
Invert 

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

