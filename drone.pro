######################################################################
# Automatically generated by qmake (1.07a) Sat May 22 15:57:15 2004
######################################################################

# Uncomment to use gprof
#QMAKE_CFLAGS+=-pg
#QMAKE_CXXFLAGS+=-pg
#QMAKE_LFLAGS+=-pg


#QMAKE_CFLAGS   +=-DDEBUG_LEVEL=0
#QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=0 -march=pentium4 -funroll-loops -fprefetch-loop-arrays -fomit-frame-pointer -pipe
#QMAKE_LFLAGS   +=-DDEBUG_LEVEL=0

#QMAKE_CFLAGS   +=-DDEBUG_LEVEL=2
#QMAKE_CXXFLAGS +=-DDEBUG_LEVEL=2
#QMAKE_LFLAGS   +=-DDEBUG_LEVEL=2

TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += . src src/gears src/types /usr/include/libmpeg3 src/stk-4.1.3/include
LIBS += -Lsrc/stk-4.1.3/src/ -lstk -lsndfile -lfftw3 -lXxf86vm -lmpeg3 -lGLU -lportaudio -lGL -lXv
CONFIG += thread 

HEADERS += config.h \
src/error.h \
src/CircularBuffer.h \
src/ColorSpace.h \
src/ConnectionItem.h \
src/Engine.h \
src/FrequencyAnalyzer.h \
src/Gear.h \
src/types/AbstractType.h \
src/types/ValueType.h \
src/types/VideoType.h \
src/types/SignalType.h \
src/types/DisplaceGrid.h \
src/types/MatrixType.h \
src/gears/Gear_AffineTransform.h \
src/gears/Gear_AudioInput.h \
src/gears/Gear_AudioOutput.h \
src/gears/Gear_AudioSource.h \
src/gears/Gear_ApplyDisplaceGrid.h \
src/gears/Gear_Blur.h \
src/gears/Gear_ColorAdjust.h \
src/gears/Gear_Contrast.h \
src/gears/Gear_ColorQuantize.h \
src/gears/Gear_ClusteredDither.h \
src/gears/Gear_DataOut.h \
src/gears/Gear_DiffDist.h \
src/gears/Gear_Edge.h \
src/gears/Gear_Enveloppe.h \
src/gears/Gear_FilterSignal.h \
src/gears/Gear_FreqBoxes.h \
src/gears/Gear_Gain.h \
src/gears/Gear_GenDisplaceGrid.h \
src/gears/Gear_GrayScale.h \
src/gears/Gear_HalfToning.h \
src/gears/Gear_Image.h \
src/gears/Gear_KDTree.h \
src/gears/Gear_MasterFrequency.h \
src/gears/Gear_MedianFilter.h \
src/gears/Gear_NGone.h \
src/gears/Gear_Osc.h \
src/gears/Gear_Oscilloscope.h \
src/gears/Gear_PatternGen.h \
src/gears/Gear_Rescale.h \
src/gears/Gear_PushButton.h \
src/gears/Gear_Saturation.h \
src/gears/Gear_SignalStat.h \
src/gears/Gear_SimpleDelay.h \
src/gears/Gear_Slider.h \
src/gears/Gear_TV.h \
src/gears/Gear_FlatSignal.h \
src/gears/Gear_VideoAdd.h \
src/gears/Gear_VideoBlend.h \
src/gears/Gear_VideoInput.h \
src/gears/Gear_VideoDelay.h \
src/gears/Gear_VideoLoop.h \
src/gears/Gear_VideoMix.h \
src/gears/Gear_VideoOutput.h \
src/gears/Gear_VideoSource.h \
src/GearGui.h \
src/GearConverter.h \
src/gears/GearGui_PushButton.h \
src/gears/GearGui_Slider.h \
src/gears/GearGui_TV.h \
src/GearMaker.h \
src/GearPropertiesDialog.h \
src/MainWindow.h \
src/Math.h \
src/AbstractPlug.h \
src/Plug.h \
src/PlugBox.h \
src/Properties.h \
src/PropertyControl.h \
src/PropertyControlFilename.h \
src/PropertyControlString.h \
src/PropertyControlBool.h \
src/Rasterer.h \
src/SummedAreaTable.h \
src/SchemaEditor.h \
src/SignalInfo.h \
src/Timing.h \
src/Typedefs.h \
src/VideoInfo.h \	   
src/VideoOutput.h \
src/VideoOutputMaker.h \
src/VideoOutputX11Base.h \
src/VideoOutputXv.h \
src/VideoOutputShm.h \
src/VideoOutputGl.h \
src/Play.xpm \
src/Pause.xpm \
src/splash.xpm \
COPYING	   

SOURCES += src/error.cpp \
src/ConnectionItem.cpp \
src/drone.cpp \
src/Engine.cpp \
src/FrequencyAnalyzer.cpp \
src/Gear.cpp \
src/types/VideoType.cpp \
src/gears/Gear_AffineTransform.cpp \
src/gears/Gear_ApplyDisplaceGrid.cpp \
src/gears/Gear_AudioInput.cpp \
src/gears/Gear_AudioOutput.cpp \
src/gears/Gear_AudioSource.cpp \
src/gears/Gear_Blur.cpp \
src/gears/Gear_ColorAdjust.cpp \
src/gears/Gear_Contrast.cpp \
src/gears/Gear_ColorQuantize.cpp \
src/gears/Gear_ClusteredDither.cpp \
src/gears/Gear_DataOut.cpp \
src/gears/Gear_DiffDist.cpp \
src/gears/Gear_Edge.cpp \
src/gears/Gear_Enveloppe.cpp \
src/gears/Gear_FilterSignal.cpp \
src/gears/Gear_FreqBoxes.cpp \
src/gears/Gear_Gain.cpp \
src/gears/Gear_GenDisplaceGrid.cpp \
src/gears/Gear_GrayScale.cpp \
src/gears/Gear_HalfToning.cpp \
src/gears/Gear_Image.cpp \
src/gears/Gear_KDTree.cpp \
src/gears/Gear_MasterFrequency.cpp \
src/gears/Gear_MedianFilter.cpp \
src/gears/Gear_NGone.cpp \
src/gears/Gear_Osc.cpp \
src/gears/Gear_Oscilloscope.cpp \
src/gears/Gear_PushButton.cpp \
src/gears/Gear_PatternGen.cpp \
src/gears/Gear_Rescale.cpp \
src/gears/Gear_Saturation.cpp \
src/gears/Gear_SignalStat.cpp \
src/gears/Gear_SimpleDelay.cpp \
src/gears/Gear_Slider.cpp \
src/gears/Gear_TV.cpp \
src/gears/Gear_FlatSignal.cpp \
src/gears/Gear_VideoAdd.cpp \
src/gears/Gear_VideoBlend.cpp \
src/gears/Gear_VideoInput.cpp \
src/gears/Gear_VideoDelay.cpp \
src/gears/Gear_VideoLoop.cpp \
src/gears/Gear_VideoMix.cpp \
src/gears/Gear_VideoOutput.cpp \
src/gears/Gear_VideoSource.cpp \
src/GearGui.cpp \
src/gears/GearGui_PushButton.cpp \
src/gears/GearGui_Slider.cpp \
src/gears/GearGui_TV.cpp \
src/GearMaker.cpp \
src/GearPropertiesDialog.cpp \
src/MainWindow.cpp \
src/AbstractPlug.cpp \
src/Plug.cpp \
src/PlugBox.cpp \
src/Properties.cpp \
src/PropertyControl.cpp \
src/PropertyControlFilename.cpp \
src/PropertyControlString.cpp \
src/PropertyControlBool.cpp \
src/Rasterer.cpp \
src/SchemaEditor.cpp \
src/SignalInfo.cpp \
src/SummedAreaTable.cpp \
src/Timing.cpp \
src/VideoInfo.cpp \	   
src/VideoOutputMaker.cpp \
src/VideoOutputX11Base.cpp \
src/VideoOutputXv.cpp \
src/VideoOutputGl.cpp \
src/VideoOutputShm.cpp

