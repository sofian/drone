#ifndef GEAR_VIDEODELAY_INCLUDED
#define GEAR_VIDEODELAY_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"

template <class T>
class CircularBuffer;

class Gear_VideoDelay : public Gear
{
public:

  Gear_VideoDelay(Engine *engine, std::string name);
  virtual ~Gear_VideoDelay();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_MEMORY;
  PlugIn<ValueType> *_DELAY;
  PlugIn<ValueType> *_RECORD;

  //local var
  const VideoTypeRGBA *_image;     
  VideoTypeRGBA *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;
  int _delay;
  int _memory;

  CircularBuffer<RGBA> * circbuf;

  void init();    
};

#endif
