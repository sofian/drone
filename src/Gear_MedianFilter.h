#ifndef GEAR_MEDIANFILTER_INCLUDED
#define GEAR_MEDIANFILTER_INCLUDED


#include "Gear.h"
#include "SummedAreaTable.h"


class Gear_MedianFilter : public Gear
{
public:

  Gear_MedianFilter(Engine *engine, std::string name);
  virtual ~Gear_MedianFilter();
  
  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

  bool ready();

private:

	PlugVideoIn *_VIDEO_IN;
  //PlugVideoIn *_AMOUNT_MAP_IN;
	PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;
  
  //local var
  Video_T *_image;     
  Video_T *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;
    
  int _x1,_y1,_x2,_y2;

  int _medianSize;
  int _halfMedianFilterSize;

  unsigned char _medianSelect[1024];
};

#endif
