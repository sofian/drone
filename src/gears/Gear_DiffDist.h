#ifndef GEAR_DIFFDIST_INCLUDED
#define GEAR_DIFFDIST_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_DiffDist : public Gear
{
public:

  Gear_DiffDist(Engine *engine, std::string name);
  virtual ~Gear_DiffDist();

  void runVideo();
  bool ready();

private:

  PlugIn<ValueType> *_FACTOR_IN;
  PlugIn<VideoTypeRGBA> *_VIDEO_IN_A;
  PlugIn<VideoTypeRGBA> *_VIDEO_IN_B;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  //local var
  MatrixType<RGBA> *_imageA; 
  MatrixType<RGBA> *_imageB; 
  MatrixType<RGBA> *_outImage; 
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


};

#endif
