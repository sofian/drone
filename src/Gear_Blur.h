#ifndef GEAR_BLUR_INCLUDED
#define GEAR_BLUR_INCLUDED


#include "Gear.h"
#include "SummedAreaTable.h"


class Gear_Blur : public Gear
{
public:

  Gear_Blur(Engine *engine, std::string name);
  virtual ~Gear_Blur();
    
  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

  bool ready();

private:

	PlugVideoIn *_VIDEO_IN;
  PlugVideoIn *_AMOUNT_MAP_IN;
	PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;

  //local var
  Video_T *_image;     
  Video_T *_outImage; 
  unsigned char *_data;
  unsigned char *_amountMapData;

  SummedAreaTable *_table;
  RGBAint _sum;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;
    
  int _x1,_y1,_x2,_y2;

  int _blurSize;
  int _homeWeight;
  int _halfBlurSize;

  RGBA _acc;

  Canvas _defaultAmountMapData;

  void init();    
};

#endif
