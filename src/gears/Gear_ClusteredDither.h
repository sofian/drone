#ifndef GEAR_CLUSTEREDDITHER_INCLUDED
#define GEAR_CLUSTEREDDITHER_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "Utils.h"

#include <stdio.h>
#include <math.h>
#include <algorithm>

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
  Matrix<double> _theta; // precomputed theta
  
  void computeThreshold();
  void computePolarCoordinates();
  float spot(float x, float y);

  inline unsigned char getValue(unsigned char intensity, int rx, int ry);
  inline int isNeg(int x) { return ((x < 0)? 1 : 0); }
};

unsigned char Gear_ClusteredDither::getValue(unsigned char intensity, int rx, int ry)
{
  ASSERT_WARNING(rx >= 0 && rx <= _width-1);
  ASSERT_WARNING(ry >= 0 && ry <= _width-1);
            
  int sum = 0;

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
