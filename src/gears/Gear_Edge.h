#ifndef GEAR_EDGE_INCLUDED
#define GEAR_EDGE_INCLUDED


#include "Gear.h"


class Gear_Edge : public Gear
{
public:

  Gear_Edge(Engine *engine, std::string name);
  virtual ~Gear_Edge();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  unsigned char *_data;
  unsigned char *_outData;

  int _sizeY;
  int _sizeX;    

  RGBA _adjust[2];

  double *_mmxImageInL0;
  double *_mmxImageInL02;
  double *_mmxImageInL1;
  double *_mmxImageInL12;
  double *_mmxImageInL2;
  double *_mmxImageInL22;
  double *_mmxImageOut;    
  double *_mmxAdjust;



};

#endif
