#ifndef GEAR_GRAYSCALE_INCLUDED
#define GEAR_GRAYSCALE_INCLUDED


#include "Gear.h"


class Gear_GrayScale : public Gear
{
public:

  Gear_GrayScale(Engine *engine, std::string name);
  virtual ~Gear_GrayScale();
    
  void runVideo();

  PlugIn<VideoType>* VIDEO_IN() { return _VIDEO_IN; }
  PlugOut<VideoType>* VIDEO_OUT() { return _VIDEO_OUT; }
  
  bool ready();

private:

  PlugIn<VideoType> *_VIDEO_IN;
  PlugOut<VideoType> *_VIDEO_OUT;

  //local var
  Canvas *_image; 
  Canvas *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _size;
            
  unsigned int *_imageOut;
  unsigned char *_imageIn;
    
    
        
};

#endif
