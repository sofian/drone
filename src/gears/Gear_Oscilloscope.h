#ifndef GEAR_OSCILLOSCOPE_INCLUDED
#define GEAR_OSCILLOSCOPE_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "CircularBufferSignal.h"

class PlugVideo;
class PlugSignal;

class Gear_Oscilloscope : public Gear
{
public:

  Gear_Oscilloscope(Engine *engine, std::string name);
  virtual ~Gear_Oscilloscope();

  void runAudio();
  void runVideo();

  bool ready();

private:

  PlugIn<SignalType> *_AUDIO_IN;
  PlugIn<ValueType> *_ZOOM_X;
  PlugIn<ValueType> *_ZOOM_Y;
  PlugIn<ValueType> *_SIZE_X;
  PlugIn<ValueType> *_SIZE_Y;
  
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  VideoTypeRGBA *_outImage; 
  RGBA *_outData;

  CircularBufferSignal * circbuf;
};

#endif
