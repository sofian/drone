#ifndef GEAR_APPLYDISPLACEGRID_INCLUDED
#define GEAR_APPLYDISPLACEGRID_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "DisplaceGrid.h"

class Gear_ApplyDisplaceGrid : public Gear
{
public:

  Gear_ApplyDisplaceGrid(Engine *engine, std::string name);
  virtual ~Gear_ApplyDisplaceGrid();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<DisplaceGrid> *_GRID;

  //local var
  const VideoTypeRGBA *_image;     
  VideoTypeRGBA *_outImage; 
  unsigned char *_data;

  const DisplaceGrid *_grid;

  unsigned char *_inData;
  unsigned char *_outData;
  float * _gridData;

  int _nx,_ny;

  int _imsizeY;
  int _imsizeX;

  int _gridsizeY;
  int _gridsizeX;

  void init();    
};

#endif
