#ifndef GEAR_VIDEOBLEND_INCLUDED
#define GEAR_VIDEOBLEND_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_VideoBlend : public Gear
{
public:

  Gear_VideoBlend(Engine *engine, std::string name);
  virtual ~Gear_VideoBlend();

  void runVideo();  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN_A;
  PlugIn<VideoTypeRGBA> *_VIDEO_IN_B;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_ALPHA_IN;

  //local var
  VideoTypeRGBA *_imageA; 
  VideoTypeRGBA *_imageB; 
  VideoTypeRGBA *_outImage; 
  RGBA *_dataA;
  RGBA *_dataB;
  RGBA *_outData;

  int _iterSizeY;
  int _iterSizeX;    
  int _imageASizeX;
  int _imageASizeY;
  int _imageBSizeX;
  int _imageBSizeY;
  int _imageOutSizeX;
  int _imageOutSizeY;

  double *_mmxImageInA;
  double *_mmxImageInB;
  double *_mmxImageOut;
  unsigned char _mmxAlpha, _mmxAlpha2;
};

#endif
