include(../../config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
TV \
Oscilloscope \
VideoOutput \
#AudioPushButton \
AlphaMix \
Blur \
VideoSource \
Slider \
KDTree\ 
VideoMix \
Edge \
AffineTransform \
ApplyDisplaceGrid \
#AreaArrayMask \
Brightness \
ClusteredDither \
ColorAdjust \
Contrast \
Enveloppe \
FlatSignal \
FillAlpha \
ListBox \
ImageCapture \
PushButton \
Rescale \
TouchPad \
TapTempo \
ColorQuantize \
Gamma \
GenDisplaceGrid \
GrayScale \
HalfToning \
ImageSource \
Invert \
InvertAlpha \
PackList \
OscMessage \
OscInput \
OscOutput \
OscFilter \
Saturation \
SelectFrame \
SetAlpha \
SignalStat \
SmearGrid \
Sobel \
StringOutput \
Tempo \
VideoDelay \
VideoRecord \
VideoOffset \
VideoSwitch \
StringGen \
StringFormat \
ValueMix
agg {
SUBDIRS+= SVGLoader \
Vectorial2Raster \
VecAffineTrans
}

#linux specific gears
unix:!macx:SUBDIRS += VideoInput AudioInput

