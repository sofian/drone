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

  PlugIn<VideoTypeRGBA>* VIDEO_IN(){return _VIDEO_IN;};
  PlugOut<VideoTypeRGBA>* VIDEO_OUT(){return _VIDEO_OUT;};

  PlugIn<SignalType>* RED_IN(){return _RED_IN;};
  PlugIn<SignalType>* GREEN_IN(){return _GREEN_IN;};
  PlugIn<SignalType>* BLUE_IN(){return _BLUE_IN;};

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<SignalType> *_RED_IN;
  PlugIn<SignalType> *_GREEN_IN;
  PlugIn<SignalType> *_BLUE_IN;

  //local var
  MatrixType<RGBA> *_image; 
  MatrixType<RGBA> *_outImage; 
  // RGBA *_data;
//   RGBA *_outData;

//   int _iterSizeY;
//   int _iterSizeX;
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

