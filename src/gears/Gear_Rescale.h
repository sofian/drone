#ifndef GEAR_RESCALE_INCLUDED
#define GEAR_RESCALE_INCLUDED


#include "Gear.h"
#include "VideoType.h"
#include "ValueType.h"
#include "Utils.h"

class Gear_Rescale : public Gear
{
public:

  Gear_Rescale(Engine *engine, std::string name);
  virtual ~Gear_Rescale();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_WIDTH_IN;
  PlugIn<ValueType> *_HEIGHT_IN;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 

  int _newWidth;
  int _newHeight;
};

#endif
