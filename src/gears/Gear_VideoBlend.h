#ifndef GEAR_VIDEOBLEND_INCLUDED
#define GEAR_VIDEOBLEND_INCLUDED


#include "Gear.h"


class Gear_VideoBlend : public Gear
{
public:

  Gear_VideoBlend(Engine *engine, std::string name);
  virtual ~Gear_VideoBlend();

  void runVideo();

  PlugVideoIn* VIDEO_IN_A(){return _VIDEO_IN_A;};
  PlugVideoIn* VIDEO_IN_B(){return _VIDEO_IN_B;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* ALPHA_IN(){return _ALPHA_IN;};


  bool ready();

private:

  PlugVideoIn *_VIDEO_IN_A;
  PlugVideoIn *_VIDEO_IN_B;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_ALPHA_IN;

  //local var
  Video_T *_imageA; 
  Video_T *_imageB; 
  Video_T *_outImage; 
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
