#ifndef GEAR_TV_INCLUDED
#define GEAR_TV_INCLUDED


#include "Gear.h"




class Gear_TV : public Gear
{
public:

  Gear_TV(Engine *engine, std::string name);
  virtual ~Gear_TV();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};

  bool ready();

protected:
  GearGui *createGearGui(QCanvas *canvas);
private:

  PlugVideoIn *_VIDEO_IN;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  unsigned char *_data;
  unsigned char *_tempData;
  RGBA *_outData;
  int _accR,_accG, _accB;

  int _sizeY;
  int _sizeX;
  int _iterSizeY;
  int _iterSizeX;
  int _startX, _startY, _endX, _endY;

  int _blurSize;
  int _halfTVSize;

  RGBA _acc;



};

#endif
