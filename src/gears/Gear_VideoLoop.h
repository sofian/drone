#ifndef GEAR_VIDEOLOOP_INCLUDED
#define GEAR_VIDEOLOOP_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"

template <class T>
class CircularBuffer;

class Gear_VideoLoop : public Gear
{
public:

  Gear_VideoLoop(Engine *engine, std::string name);
  virtual ~Gear_VideoLoop();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_PUNCH_OUT;
  PlugIn<ValueType> *_PUNCH_IN;
  PlugIn<ValueType> *_MEMORY;

  //local var
  const VideoTypeRGBA *_image;     
  VideoTypeRGBA *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;

  CircularBuffer<RGBA> * circbuf;

  void init();    
};

#endif
