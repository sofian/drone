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

#define ISNEG(x)	(((x) < 0)? 1 : 0)

class Gear_ClusteredDither : public Gear
{
  enum eSpotType { SQUARE = 0, DIAMOND = 1, ROUND = 2, LINE = 3 };
  static const std::string SETTING_SPOT_FUNCTION;
  
public:

  Gear_ClusteredDither(Engine *engine, std::string name);
  virtual ~Gear_ClusteredDither();
    
  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};
  
  bool ready();

  void init();

protected:
  void onUpdateSettings();
  
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

  eSpotType _spotType;
  
  inline void computeThreshold();
  inline unsigned char getValue(int intensity, int rx, int ry);
  inline float spot(float x, float y);
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
      float val = spot(sx, sy);

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

unsigned char Gear_ClusteredDither::getValue(int intensity, int rx, int ry)
{
  int sum = 0;

  int ryLow = ry-1;
  if (ryLow < 0) ryLow += _width;

  int ryHigh = ry+1;
  if (ryHigh >= _width) ryHigh -= _width;

  int rxLow = rx-1;
  if (rxLow < 0) rxLow += _width;

  int rxHigh = rx+1;
  if (rxHigh >= _width) rxHigh -= _width;

  // This obscure code is based on an unrolling of the antialiasing loop. The
  // numbers (255, 510, 1010) are taken from the Bartlett matrix.
  
  if (intensity > _threshold[ryLow*_width + rxLow])
    sum += 255;

  if (intensity > _threshold[ryLow*_width + rx])
    sum += 510;

  if (intensity > _threshold[ryLow*_width + rxHigh])
    sum += 255;

  if (intensity > _threshold[ry*_width + rxLow])
    sum += 510;

  if (intensity > _threshold[ry*_width + rx])
    sum += 1010;

  if (intensity > _threshold[ry*_width + rxHigh])
    sum += 510;

  if (intensity > _threshold[ryHigh*_width + rxLow])
    sum += 255;
  
  if (intensity > _threshold[ryHigh*_width + rx])
    sum += 510;

  if (intensity > _threshold[ryHigh*_width + rxHigh])
    sum += 255;

  sum >>= 4;

  return (unsigned char) sum;
}

float Gear_ClusteredDither::spot(float x, float y)
{
  switch (_spotType)
  {
   /* The following functions were derived from a peice of PostScript by
    * Peter Fink and published in his book, "PostScript Screening: Adobe
    * Accurate Screens" (Adobe Press, 1992).  Adobe Systems Incorporated
    * allow its use, provided the following copyright message is present:
    *
    *  % Film Test Pages for Screenset Development
    *  % Copyright (c) 1991 and 1992 Adobe Systems Incorporated
    *  % All rights reserved.
    *  %
    *  % NOTICE: This code is copyrighted by Adobe Systems Incorporated, and
    *  % may not be reproduced for sale except by permission of Adobe Systems
    *  % Incorporated. Adobe Systems Incorporated grants permission to use
    *  % this code for the development of screen sets for use with Adobe
    *  % Accurate Screens software, as long as the copyright notice remains
    *  % intact.
    *  %
    *  % By Peter Fink 1991/1992
    */

   /* Square (or Euclidean) dot.  Also very common. */
  case SQUARE:
    {
      float ax = fabs(x);
      float ay = fabs(y);
      
      return (ax+ay)>1? ((ay-1)*(ay-1) + (ax-1)*(ax-1)) - 1 : 1-(ay*ay + ax*ax);
    }
    
    /* Diamond spot function, again from Peter Fink's PostScript
     * original.  Copyright as for previous function. */
  case DIAMOND:
    {
      float ax = fabs(x);
      float ay = fabs(y);

      return (ax+ay)<=0.75? 1-(ax*ax + ay*ay) :	// dot
        ( (ax+ay)<=1.23?  1-((ay*0.76) + ax) :	// to diamond
          ((ay-1)*(ay-1) + (ax-1)*(ax-1)) -1);	// back to dot
    }

    /* Another commonly seen spot function is the v-shaped wedge. Tonally
     * balanced. */
  case LINE:
   return fabs(y);
   
   /* The classic growing dot spot function. */
  case ROUND:
  default:
    return (1. - x*x - y*y);
  }
}

#endif
