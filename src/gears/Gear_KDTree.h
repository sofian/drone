#ifndef GEAR_KDTREE_INCLUDED
#define GEAR_KDTREE_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include <assert.h>
#include "Timing.h"
#include "Rasterer.h"
#include "SummedAreaTable.h"

class Gear_KDTree : public Gear  
{

public:
  Gear_KDTree(Engine *engine, std::string name);
  virtual ~Gear_KDTree();

  void runVideo();  bool ready();

  void init();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  MatrixType<RGBA> *_image; 
  MatrixType<RGBA> *_outImage; 
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
