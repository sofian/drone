#ifndef GEAR_COLORADJUST_INCLUDED
#define GEAR_COLORADJUST_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_ColorAdjust : public Gear
{
public:

  Gear_ColorAdjust(Engine *engine, std::string name);
  virtual ~Gear_ColorAdjust();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_RED_IN;
  PlugIn<ValueType> *_GREEN_IN;
  PlugIn<ValueType> *_BLUE_IN;

  //local var
  VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 

  int _size;

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

