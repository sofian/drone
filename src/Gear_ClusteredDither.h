#ifndef GEAR_CLUSTEREDDITHER_INCLUDED
#define GEAR_CLUSTEREDDITHER_INCLUDED

#include "Gear.h"

#include <stdio.h>
#include <math.h>

//#define OVERSAMPLE 3

/* Each call of the spot function results in one of these */
typedef struct {
    int	index;	/* (y * width) + x */
    double value;	/* return value of the spot function */
} order_t;

/* qsort(3) compare function */
static int
order_cmp(const void *va, const void *vb)
{
    const order_t *a = (const order_t*)va;
    const order_t *b = (const order_t*)vb;

    return (a->value < b->value)? -1 : ((a->value > b->value)? +1 : 0);
}

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

private:
  PlugVideoIn *_VIDEO_IN;
	PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;

  int _clusterSize;
  
  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _sizeY;
  int _sizeX;    

  unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
