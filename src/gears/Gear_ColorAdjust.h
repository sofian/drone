#ifndef GEAR_COLORADJUST_INCLUDED
#define GEAR_COLORADJUST_INCLUDED


#include "Gear.h"


class Gear_ColorAdjust : public Gear
{
public:

  Gear_ColorAdjust(Engine *engine, std::string name);
  virtual ~Gear_ColorAdjust();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  PlugSignalIn* RED_IN(){return _RED_IN;};
  PlugSignalIn* GREEN_IN(){return _GREEN_IN;};
  PlugSignalIn* BLUE_IN(){return _BLUE_IN;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_RED_IN;
  PlugSignalIn *_GREEN_IN;
  PlugSignalIn *_BLUE_IN;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _iterSizeY;
  int _iterSizeX;    

  int *_mmxImageIn;
  int *_mmxImageOut;    
  unsigned long long int _mmxColor;

  short _r;
  short _g;
  short _b;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
