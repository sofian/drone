#ifndef GEAR_EDGE_INCLUDED
#define GEAR_EDGE_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_Edge : public Gear
{
public:

  Gear_Edge(Engine *engine, std::string name);
  virtual ~Gear_Edge();

  void runVideo();
  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 
  unsigned char *_data;
  unsigned char *_outData;

  int _sizeY;
  int _sizeX;    

  RGBA *_iterPrevRow;
  RGBA *_iterCurrRow;
  RGBA *_iterNextRow;
  RGBA *_iterOutData;

  //  RGBA _adjust[2];

//   double *_mmxImageInL0;
//   double *_mmxImageInL02;
//   double *_mmxImageInL1;
//   double *_mmxImageInL12;
//   double *_mmxImageInL2;
//   double *_mmxImageInL22;
//   double *_mmxImageOut;    
//   double *_mmxAdjust;



};

#endif
