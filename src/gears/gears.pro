include(../../config.pro)

TEMPLATE=subdirs
SUBDIRS= Osc \
TV \
Oscilloscope \
VideoOutput \
#AudioPushButton \
#AlphaMix \
#Blur \
#VideoSource \
Slider \
#KDTree\ 
#VideoMix \
#Edge \
#AffineTransform \
#ApplyDisplaceGrid \
#AreaArrayMask \
#Brightness \
#ClusteredDither \
#ColorAdjust \
#Contrast \
#Enveloppe \
FlatSignal \
#FillAlpha \
ListBox \
PushButton \
#Rescale \
#TouchPad \
#TapTempo \
#ColorQuantize \
#Gamma \
#GenDisplaceGrid \
#GrayScale \
#HalfToning \
ImageSource \
#Invert \
#InvertAlpha \
#OscInput \
#OscFilter \
#Saturation \
#SelectFrame \
#SetAlpha \
#SignalStat \
#SmearGrid \
#Sobel \
#Tempo \
#VideoDelay \
#VideoRecord \
#VideoOffset \
VideoSwitch \
ValueMix
agg {
SUBDIRS+=AggTest \
AmuseOeil
}

#linux specific gears
	unix:!macx:SUBDIRS += VideoInput AudioInput

