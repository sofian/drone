#include "Gear_ClusteredDither.h"
#include "Engine.h"
#include "Math.h"
#include <math.h> // XXX debug
#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ClusteredDither, Gear_ClusteredDither, "ClusteredDither")

Gear_ClusteredDither::Gear_ClusteredDither(Engine *engine, std::string name)
: Gear(engine, "ClusteredDither", name), _sizeX(0), _sizeY(0), _clusterSize(0),_width(0), _threshold(0), _order(0), _spotType(ROUND)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_CLUSTER_SIZE_IN = new PlugIn<ValueType>(this, "ClusterSize", new ValueType(16, 2, 32)));
  addPlug(_SPOT_TYPE_IN = new PlugIn<ValueType>(this, "SpotType", new ValueType(ROUND, SQUARE, LINE)));
  addPlug(_ANGLE_RED_IN = new PlugIn<ValueType>(this, "AngleRed", new ValueType(15,0,90)));
  addPlug(_ANGLE_GREEN_IN = new PlugIn<ValueType>(this, "AngleGreen", new ValueType(75,0,90)));
  addPlug(_ANGLE_BLUE_IN = new PlugIn<ValueType>(this, "AngleBlue", new ValueType(0,0,90)));
}

Gear_ClusteredDither::~Gear_ClusteredDither()
{
  free(_threshold);
  free(_order);
}

void Gear_ClusteredDither::init()
{
  _clusterSize = CLAMP((int)_CLUSTER_SIZE_IN->type()->value(), 2, 512);
  _spotType = (eSpotType)CLAMP((int)_SPOT_TYPE_IN->type()->value(), (int)SQUARE, (int)LINE);
  _width = _clusterSize * 3;
  computeThreshold();
  computePolarCoordinates();
  ASSERT_WARNING(_threshold);
  ASSERT_WARNING(_order);
}

void Gear_ClusteredDither::onUpdateSettings()
{
}

bool Gear_ClusteredDither::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ClusteredDither::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  
  if (_sizeX != (int)_image->width() || _sizeY != (int)_image->height())
  {
    _sizeX = _image->width();
    _sizeY = _image->height();
    computePolarCoordinates();
  }
  else
  {
    _sizeX = _image->width();
    _sizeY = _image->height();
  }
  
  _data = _image->data();    
  _outData = _outImage->data();

  _angle[0] = DEG2RAD(CLAMP((int)_ANGLE_RED_IN->type()->value(), 0, 90));
  _angle[1] = DEG2RAD(CLAMP((int)_ANGLE_GREEN_IN->type()->value(), 0, 90));
  _angle[2] = DEG2RAD(CLAMP((int)_ANGLE_BLUE_IN->type()->value(), 0, 90));
  
  //NOTICE("Angles : R=%f, G=%f, B=%f", _angle[0], _angle[1], _angle[2]);
  
  unsigned char *iterData = (unsigned char*)_data;
  unsigned char *iterOutData = (unsigned char*)_outData;

//   int maxClusterSizeX;
//   int maxClusterSizeY;
//   int minClusterSizeX;
//   int minClusterSizeY;

  bool valuesHaveChanged = false;
  // If cluster size has changed, recompute threshold matrix.
  int clusterSize = CLAMP((int)_CLUSTER_SIZE_IN->type()->value(), 2, MAX((int)_image->height(),4));
  if (_clusterSize != clusterSize)
  {
    _clusterSize = clusterSize;
    _width = _clusterSize * 3;
    valuesHaveChanged = true;
  }

  // XXX computeThreshold deux fois!!!
  // Set spot type.
  eSpotType tmpSpotType = (eSpotType)CLAMP((int)_SPOT_TYPE_IN->type()->value(), (int)SQUARE, (int)LINE);
  if (tmpSpotType != _spotType)
  {
    _spotType = tmpSpotType;
    valuesHaveChanged = true;
  }

  if (valuesHaveChanged)
    computeThreshold();

  iterData = (unsigned char*) _data;
  iterOutData = (unsigned char*) _outData;
  
  for (int y=0; y<_sizeY; ++y)
  {
    for (int x=0; x<_sizeX; ++x)
    {
      /* convert rx and ry to polar (r, theta) */
      double r = _r(x,y);
      double theta = _theta(x,y);
      
      for (int c=0; c<SIZE_RGB; ++c)
      {
        double theta_c = CLAMP(theta + _angle[c], 0.0, TWICE_PI);
        int rx = (int)rint( fastPolarToX(r,  theta_c) );
        int ry = (int)rint( fastPolarToY(r,  theta_c) );
        
        /* Make sure rx and ry are positive and within
         * the range 0 .. width-1 (incl).  Can't use %
         * operator, since its definition on negative
         * numbers is not helpful.  Can't use ABS(),
         * since that would cause reflection about the
         * x- and y-axes.  Relies on integer division
         * rounding towards zero. */
        rx -= ((rx - isNeg(rx)*(_width-1)) / _width) * _width;
        ry -= ((ry - isNeg(ry)*(_width-1)) / _width) * _width;
        
        ASSERT_WARNING(rx >= 0 && rx <= _width-1);
        ASSERT_WARNING(ry >= 0 && ry <= _width-1);
        
        //            NOTICE("rx=%d, ry=%d", rx, ry);
        // Compute and copy value.
        iterOutData[c] = getValue(iterData[c], rx, ry);
      }
      
      iterData+=SIZE_RGBA;
      iterOutData+=SIZE_RGBA;
      
    }
  }  

}

void Gear_ClusteredDither::computeThreshold()
{
  int width2 = _width*_width;

  _threshold = (unsigned char*)realloc(_threshold, width2*sizeof(unsigned char));
  _order = (Order*)realloc(_order, width2*sizeof(Order));

  Order *iterOrder = _order;

  // inside cell
  int i=0;
  for (int yCell=0; yCell<_width; ++yCell)
  {
    float sy = 2*(float)yCell / (_width-1) - 1;
    for (int xCell=0; xCell<_width; ++xCell)
    {
      float sx = 2*(float)xCell / (_width-1) - 1;
      float val = spot(sx, sy);

      *iterOrder++ = std::make_pair(i++,val);
    }
  }

  // now sort array of (point, value) pairs in ascending order
  std::sort(_order, _order+width2, less);

  /*
    _threshold[] contains values from 0 .. 254.  The reason for not
    including 255 is so that an image value of 255 remains
    unmolested.  It would be bad to filter a completely white
    image and end up with black speckles.
  */
  int val = 0;
  iterOrder = _order;
  for (i=0; i < width2; i++, val += 0xff)
    _threshold[(iterOrder++)->first] = val / width2; // *** pas besoin de .value dans c'cas là...
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

void Gear_ClusteredDither::computePolarCoordinates()
{
  _r.resize(_sizeX, _sizeY);
  _theta.resize(_sizeX, _sizeY);
  MatrixType<double>::iterator rIt = _r.begin(), tIt = _theta.begin();
  for (int y=0; y<_sizeY; ++y)
    for (int x=0; x<_sizeX; ++x)
    {
      *rIt++     = fastDist(x,y);
      *tIt++ = fastAngle(x,y);
    }
}
