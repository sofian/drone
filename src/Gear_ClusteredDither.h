#ifndef GEAR_CLUSTEREDDITHER_INCLUDED
#define GEAR_CLUSTEREDDITHER_INCLUDED

#include "Gear.h"

#include <stdio.h>
#include <math.h>

//#define OVERSAMPLE 3

/* Each call of the spot function results in one of these */
typedef struct {
    int	index;	/* (y * width) + x */
    float value;	/* return value of the spot function */
} order_t;

/* qsort(3) compare function */
static int
order_cmp(const void *va, const void *vb)
{
    const order_t *a = (const order_t*)va;
    const order_t *b = (const order_t*)vb;

    return (a->value < b->value)? -1 : ((a->value > b->value)? +1 : 0);
}

#define SPOT_ROUND(x, y) 1-x*x-y*y
//#define BARTLETT(x,y)	((2-abs(x)) * (2-abs(y)))
int BARTLETT[3][3] = {
  { 255, 510,  255 },
  { 510, 1020, 510 },
  { 255, 510,  255 },
};

#define ISNEG(x)	(((x) < 0)? 1 : 0)

class Gear_ClusteredDither : public Gear
{
public:

  Gear_ClusteredDither(Engine *engine, std::string name);
  virtual ~Gear_ClusteredDither();
    
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

  int _clusterSize;
  int _width; // clusterSize * 3
  
  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _sizeY;
  int _sizeX;    

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  unsigned char *_threshold;
  order_t *_order;

  inline void computeThreshold();
};

void Gear_ClusteredDither::computeThreshold()
{
  int width2 = _width*_width;
  
  _threshold = (unsigned char*)realloc(_threshold, width2*sizeof(unsigned char));
  _order = (order_t*)realloc(_order, width2*sizeof(order_t));

  int i=0;
  // inside cell
  for (int yCell=0; yCell<_width; ++yCell)
  {
    float sy = 2*(float)yCell / (_width-1) - 1;
    for (int xCell=0; xCell<_width; ++xCell)
    {
      float sx = 2*(float)xCell / (_width-1) - 1;
      float val = SPOT_ROUND(sx, sy);
      
      _order[i].index = i;
      _order[i].value = val;
      ++i; 
    }
  }
  
  // now sort array of (point, value) pairs in ascending order
  // *** utiliser le sort de la STL??? ou au moins order_cmp = fonction statique de classe
  qsort(_order, width2, sizeof(order_t), order_cmp);

  /*
    _threshold[] contains values from 0 .. 254.  The reason for not
    including 255 is so that an image value of 255 remains
    unmolested.  It would be bad to filter a completely white
    image and end up with black speckles.
  */
  int val = 0;
  for (i=0; i < width2; i++, val += 0xff)
    _threshold[_order[i].index] = val / width2; // *** pas besoin de .value dans c'cas là...
}

#endif
