#ifndef GEAR_OSCILLOSCOPE_INCLUDED
#define GEAR_OSCILLOSCOPE_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"

class PlugVideo;
class PlugSignal;

class Gear_Oscilloscope : public Gear
{
public:

  Gear_Oscilloscope(Engine *engine, std::string name);
  virtual ~Gear_Oscilloscope();

  void runVideo();

  bool ready();

private:

  PlugIn<SignalType> *_AUDIO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  MatrixType<RGBA> *_outImage; 
  RGBA *_outData;


};

#endif
