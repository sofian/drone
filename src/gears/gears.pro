include(../../config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
Oscilloscope \
VideoOutput \
AlphaMix \
Blur \
VideoSource \
KDTree\ 
VideoMix \
Edge \
AffineTransform \
ApplyDisplaceGrid \
AreaClip \
AreaCreate \
AreaScale \
AreaTranslate \
AreaVideoSelect \
AreaVideoReplace \
#AreaArrayMask \
#AreaArrayClip \
#AreaArrayVideoSelect \
AudioInput \
AudioOutput \
Brightness \
ClusteredDither \
ColorAdjust \
Contrast \
Enveloppe \
EmptyImage \
FlatSignal \
FillAlpha \
ImageCapture \
ImageToTexture \
Rescale \
ColorQuantize \
Gamma \
GenDisplaceGrid \
GrayScale \
HalfToning \
#EmptyImage \
#ImageSource \
#ImageSequence \
Invert \
InvertAlpha \
PackList \
UnpackList \
#OscMessage \
#OscInput \
#OscOutput \
#OscFilter \
Saturation \
SelectFrame \
SetAlpha \
SignalStat \
SmearGrid \
Sobel \
#StringGen \
StringFormat \
#StringSource \
RandomValue \
Tempo \
ValueClamp \
VideoDelay \
#VideoInput \
VideoRecord \
VideoSource \
VideoOffset \
#VideoReplaceAreas \
VideoSwitch \
ValueMix

agg {
SUBDIRS+= SVGLoader \
Vectorial2Raster \
VecAffineTrans
}

#Controls

SUBDIRS+= controls/PushButton

#linux specific gears
#unix:!macx:SUBDIRS += VideoInput AudioInput
unix:!macx:SUBDIRS += VideoInput

