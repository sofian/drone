#ifndef GEAR_VIDEOADD_INCLUDED
#define GEAR_VIDEOADD_INCLUDED


#include "Gear.h"
#include "VideoType.h"


class Gear_VideoAdd : public Gear
{
public:

  Gear_VideoAdd(Engine *engine, std::string name);
  virtual ~Gear_VideoAdd();

  void runVideo();  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN_A;
  PlugIn<VideoTypeRGBA> *_VIDEO_IN_B;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  //local var
  VideoTypeRGBA *_imageA; 
  VideoTypeRGBA *_imageB; 
  VideoTypeRGBA *_outImage; 
  RGBA *_dataA;
  RGBA *_dataB;
  RGBA *_outData;
  RGBA *_largerImage;
  RGBA *_tallerImage;

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

  int _remainIterSizeY;
  int _remainIterSizeX;

};

#endif
