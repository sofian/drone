#ifndef GEAR_CONTRAST_INCLUDED
#define GEAR_CONTRAST_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_Contrast : public Gear
{
public:

  Gear_Contrast(Engine *engine, std::string name);
  virtual ~Gear_Contrast();

  void runVideo();  bool ready();

private:


  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_CONTRAST_IN;

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
