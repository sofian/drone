#ifndef GEAR_GRAYSCALE_INCLUDED
#define GEAR_GRAYSCALE_INCLUDED


#include "Gear.h"
#include "VideoType.h"


class Gear_GrayScale : public Gear
{
public:

  Gear_GrayScale(Engine *engine, std::string name);
  virtual ~Gear_GrayScale();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  
  std::vector<PlugIn<VideoTypeRGBA>* > _SHIT;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 

  int _size;

  unsigned int *_imageOut;
  unsigned char *_imageIn;



};

#endif
