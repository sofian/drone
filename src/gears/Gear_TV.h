#ifndef GEAR_TV_INCLUDED
#define GEAR_TV_INCLUDED


#include "Gear.h"
#include "VideoType.h"




class Gear_TV : public Gear
{
public:

  Gear_TV(Engine *engine, std::string name);
  virtual ~Gear_TV();

  PlugIn<VideoTypeRGBA>* VIDEO_IN(){return _VIDEO_IN;};

  void runVideo();
  bool ready();

protected:
  GearGui *createGearGui(QCanvas *canvas);
private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;

  //local var
  MatrixType<RGBA> *_image; 
  MatrixType<RGBA> *_outImage; 
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
