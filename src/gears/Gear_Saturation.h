#ifndef GEAR_SATURATION_INCLUDED
#define GEAR_SATURATION_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_Saturation : public Gear
{
public:

  Gear_Saturation(Engine *engine, std::string name);
  virtual ~Gear_Saturation();

  void runVideo();  bool ready();

private:


  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _iterSizeY;
  int _iterSizeX;    

  short _r;
  short _g;
  short _b;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
