#ifndef GEAR_CLUSTEREDDITHER_INCLUDED
#define GEAR_CLUSTEREDDITHER_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "Utils.h"

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>

// XXX indiquer la source (c.f. sur le wiki)

class Gear_ClusteredDither : public Gear
{
  enum eSpotType
  {
    SQUARE, DIAMOND, ROUND, LINE
  };

  typedef std::pair<int, float> Order;
  static bool less(const Order& a, const Order& b) { return(a.second < b.second);}

public:

  Gear_ClusteredDither(Engine *engine, std::string name);
  virtual ~Gear_ClusteredDither();

  void runVideo();

  bool ready();

  void init();

protected:
  void onUpdateSettings();

private:
  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_CLUSTER_SIZE_IN;
  PlugIn<ValueType> *_SPOT_TYPE_IN;
  PlugIn<ValueType> *_ANGLE_RED_IN;
  PlugIn<ValueType> *_ANGLE_GREEN_IN;
  PlugIn<ValueType> *_ANGLE_BLUE_IN;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 
  const RGBA *_data;
  RGBA *_outData;

  int _sizeX;    
  int _sizeY;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  int _clusterSize;
  int _width; // clusterSize * 3

  unsigned char *_threshold;
  Order *_order;

  eSpotType _spotType;

  double _angle[SIZE_RGB];

  Matrix<double> _r;     // precomputed r
  // precomputed thetas
  Matrix<double> _theta;
  Matrix<std::pair<int, int> > _rChannel[SIZE_RGB];

  //! Internal use (updates various components that hold precomputed data).
  void updateThreshold();
  void updatePolarCoordinates();
  void updateAngle(int channel);

  //! The spot function.
  inline float spot(float x, float y);

  //! Returns the value of point with intensity #intensity#, at local (in-spot) coordinates #rx#, #ry#.
  inline unsigned char getValue(unsigned char intensity, int rx, int ry);
  inline int isNeg(int x) { return ((x < 0)? 1 : 0); }
};

float Gear_ClusteredDither::spot(float x, float y)
{
  switch (_spotType)
  {
  /* The following functions were derived from a piece of PostScript by
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

      return(ax+ay)>1? ((ay-1)*(ay-1) + (ax-1)*(ax-1)) - 1 : 1-(ay*ay + ax*ax);
    }

    /* Diamond spot function, again from Peter Fink's PostScript
     * original.  Copyright as for previous function. */
  case DIAMOND:
    {
      float ax = fabs(x);
      float ay = fabs(y);

      return(ax+ay)<=0.75? 1-(ax*ax + ay*ay) : // dot
      ( (ax+ay)<=1.23?  1-((ay*0.76) + ax) :  // to diamond
        ((ay-1)*(ay-1) + (ax-1)*(ax-1)) -1);  // back to dot
    }

    /* Another commonly seen spot function is the v-shaped wedge. Tonally
     * balanced. */
  case LINE:
    return fabs(y);

    /* The classic growing dot spot function. */
  case ROUND:
  default:
    return(1. - x*x - y*y);
  }
}

unsigned char Gear_ClusteredDither::getValue(unsigned char intensity, int rx, int ry)
{
  ASSERT_WARNING(rx >= 0 && rx <= _width-1);
  ASSERT_WARNING(ry >= 0 && ry <= _width-1);
            
  int sum = 0;

  // Set coordinates for antialiasing.

  int ryLow = ry-1;
  if (ryLow < 0) ryLow += _width;
  
  int ryHigh = ry+1;
  if (ryHigh >= _width) ryHigh -= _width;

  int rxLow = rx-1;
  if (rxLow < 0) rxLow += _width;

  int rxHigh = rx+1;
  if (rxHigh >= _width) rxHigh -= _width;

  ryLow *=_width;
  ry*=_width;
  ryHigh*=_width;

  // This obscure code is based on an unrolling of the antialiasing loop. The
  // numbers (255, 510, 1010) are taken from the Bartlett matrix.

  if (intensity > _threshold[ryLow  + rxLow])  sum += 255;
  if (intensity > _threshold[ryLow  + rx])     sum += 510;
  if (intensity > _threshold[ryLow  + rxHigh]) sum += 255;
  if (intensity > _threshold[ry     + rxLow])  sum += 510;
  if (intensity > _threshold[ry     + rx])     sum += 1010;
  if (intensity > _threshold[ry     + rxHigh]) sum += 510;
  if (intensity > _threshold[ryHigh + rxLow])  sum += 255;
  if (intensity > _threshold[ryHigh + rx])     sum += 510;
  if (intensity > _threshold[ryHigh + rxHigh]) sum += 255;

  sum >>= 4; // divide by 16

  return(unsigned char) sum;
}

#endif
