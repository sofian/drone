#ifndef GEAR_KDTREE_INCLUDED
#define GEAR_KDTREE_INCLUDED

#include "Gear.h"
#include <assert.h>
#include "Timing.h"
#include "Rasterer.h"
#include "SummedAreaTable.h"

class Gear_KDTree : public Gear  
{
  
public:
  Gear_KDTree(Engine *engine, std::string name);
  virtual ~Gear_KDTree();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

  bool ready();

  void init();

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  SummedAreaTable *_table;
  
  int _sizeX;
  int _sizeY;
  int _size;
  
  int _r;
  int _g;
  int _b;
    
  unsigned char *_imageIn;
  unsigned char *_imageOut;

  Rasterer *_rasterer;

  int _maxDepth;

  void split(int x0, int x1, int y0, int y1, int depth);
};

#endif 
