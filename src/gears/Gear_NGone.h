#ifndef GEAR_NGONE_INCLUDED
#define GEAR_NGONE_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_NGone : public Gear
{
public:

  Gear_NGone(Engine *engine, std::string name);
  virtual ~Gear_NGone();

  void runVideo();  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugIn<SignalType> *_SIGNAL_X_IN;
  PlugIn<SignalType> *_SIGNAL_Y_IN;

  //locals
  VideoTypeRGBA *_image;

  float _sizeX;
  float _sizeY;

  float _texSizeX;
  float _texSizeY;

  float *_signalBufferX;
  float *_signalBufferY;

};

#endif
