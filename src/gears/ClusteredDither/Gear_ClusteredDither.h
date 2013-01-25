/* Gear_ClusteredDither.h
 * Copyright (C) 2004, 2012 Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GEAR_CLUSTEREDDITHER_INCLUDED
#define GEAR_CLUSTEREDDITHER_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "EnumType.h"
#include "ValueType.h"
#include "Utils.h"

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>

/**
 * This class implements an optimized version of the newsprint plugin for the Gimp
 * (see http://www.cl.cam.ac.uk/~and1000/newsprint/index.html). It simulates a dot
 * matrix printer.
 *
 * @author Jean-Sebastien Senecal
 * version %I% %G%
 */

class Gear_ClusteredDither : public Gear
{
  //! The spot type of the printer.
  enum eSpotType
  {
    SQUARE, DIAMOND, ROUND, LINE, N_SPOT_TYPES
  };

  // Internal use.
  typedef std::pair<int, float> Order;
  static bool less(const Order& a, const Order& b) { return(a.second < b.second);}

public:
  //! Default constructor.
  Gear_ClusteredDither();

  // Destructor.
  virtual ~Gear_ClusteredDither();

  void runVideo();

protected:
  void internalInit();
  void onUpdateSettings();

private:
  //! Video input.
  PlugIn<VideoRGBAType> *_VIDEO_IN;

  //! Video output.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  //! The size of the cluster.
  PlugIn<ValueType> *_CLUSTER_SIZE_IN;

  //! The type of printer.
  PlugIn<EnumType> *_SPOT_TYPE_IN;

  //! The angle for the red channel, in degrees.
  PlugIn<ValueType> *_ANGLE_RED_IN;

  //! The angle for the green channel, in degrees.
  PlugIn<ValueType> *_ANGLE_GREEN_IN;
  
  //! The angle for the blue channel, in degrees.
  PlugIn<ValueType> *_ANGLE_BLUE_IN;

  // Internal use.
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  const RGBA *_data;
  RGBA *_outData;

  int _sizeX;    
  int _sizeY;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  int _clusterSize;
  int _clusterWidth; // clusterSize * 3

  unsigned char *_threshold;
  Order *_order;

  eSpotType _spotType;

  double _angle[SIZE_RGB];

  // Precomputed r.
  Array2D<double> _r;
  
  // Precomputed thetas.
  Array2D<double> _theta;
  Array2D<std::pair<int, int> > _rChannel[SIZE_RGB];

  // Internal use (updates various components that hold precomputed data).
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
  ASSERT_WARNING(rx >= 0 && rx <= _clusterWidth-1);
  ASSERT_WARNING(ry >= 0 && ry <= _clusterWidth-1);
            
  int sum = 0;

  // Set coordinates for antialiasing.

  int ryLow = ry-1;
  if (ryLow < 0) ryLow += _clusterWidth;
  
  int ryHigh = ry+1;
  if (ryHigh >= _clusterWidth) ryHigh -= _clusterWidth;

  int rxLow = rx-1;
  if (rxLow < 0) rxLow += _clusterWidth;

  int rxHigh = rx+1;
  if (rxHigh >= _clusterWidth) rxHigh -= _clusterWidth;

  ryLow *=_clusterWidth;
  ry*=_clusterWidth;
  ryHigh*=_clusterWidth;

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
