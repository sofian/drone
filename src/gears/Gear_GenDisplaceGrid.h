#ifndef GEAR_GENDISPLACEGRID_INCLUDED
#define GEAR_GENDISPLACEGRID_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "DisplaceGrid.h"
#include "VideoType.h"

class Gear_GenDisplaceGrid : public Gear
{
public:

  Gear_GenDisplaceGrid(Engine *engine, std::string name);
  virtual ~Gear_GenDisplaceGrid();

  void runVideo();

  bool ready();

private:

  PlugOut<DisplaceGrid> *_GRID_OUT;
  PlugIn<ValueType> *_TYPE;
  PlugIn<ValueType> *_PARAM1;
  PlugIn<ValueType> *_PARAM2;
  PlugIn<ValueType> *_PARAM3;
  PlugIn<ValueType> *_SCALE;
  PlugIn<VideoTypeRGBA> *_VIDEO_IN;


  //local vars
  float _param1,_param2,_param3,_scale;
  float _lastparam1,_lastparam2,_lastparam3,_lasttype,_lastscale;

  int _xsize,_ysize,_type;
  //!image halfdiagonal
  float _halfdiag;

  XYDisp *_gridData; 
  DisplaceGrid * _gridOut;
  const VideoTypeRGBA *_image;     

  unsigned char* _data; // pointer on input image

  void init();    
};

#endif
