#ifndef GEAR_BLUR_INCLUDED
#define GEAR_BLUR_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "SummedAreaTable.h"

class Gear_Blur : public Gear
{
public:

  Gear_Blur(Engine *engine, std::string name);
  virtual ~Gear_Blur();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  const VideoTypeRGBA *_image;     
  VideoTypeRGBA *_outImage;
  unsigned char *_outData;

  SummedAreaTable<> *_table;
  int _sum[SIZE_RGBA];

  int _sizeY;
  int _sizeX;

  int _x1,_y1,_x2,_y2;

  int _blurSize;
  int _area;
  
  void init();    
};

#endif
