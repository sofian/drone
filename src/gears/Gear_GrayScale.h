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

  PlugIn<VideoTypeRGBA>* VIDEO_IN() { return _VIDEO_IN;}
  PlugOut<VideoTypeRGBA>* VIDEO_OUT() { return _VIDEO_OUT;}

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  //local var
  MatrixType<RGBA> *_image; 
  MatrixType<RGBA> *_outImage; 

  int _size;

  unsigned int *_imageOut;
  unsigned char *_imageIn;



};

#endif
