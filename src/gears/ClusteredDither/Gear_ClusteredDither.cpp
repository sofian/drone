/* Gear_ClusteredDither.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include "Gear_ClusteredDither.h"
#include "Engine.h"
#include "DroneMath.h"
#include <math.h> // XXX debug
#include <iostream>



extern "C" {
Gear* makeGear()
{
  return new Gear_ClusteredDither();
}


}

Gear_ClusteredDither::Gear_ClusteredDither()
: Gear("ClusteredDither"),
  _sizeX(0), _sizeY(0), _clusterSize(0), _clusterWidth(0), _threshold(0), _order(0), _spotType(ROUND)
{
  // Video I/O
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));

  // Cluster size
  addPlug(_CLUSTER_SIZE_IN = new PlugIn<ValueType>(this, "ClusterSize", false, new ValueType(2, 2, 32)));

  // Spot type
  EnumType *spotType = new EnumType((int)N_SPOT_TYPES, (int)ROUND);
  spotType->setLabel((int)SQUARE, "Square");
  spotType->setLabel((int)DIAMOND, "Diamond");
  spotType->setLabel((int)ROUND, "Round");
  spotType->setLabel((int)LINE, "Line");

  addPlug(_SPOT_TYPE_IN = new PlugIn<EnumType>(this, "SpotType", false, spotType));
  
  // Channel angles
  addPlug(_ANGLE_RED_IN = new PlugIn<ValueType>(this, "AngleRed", false, new ValueType(15,0,360)));
  addPlug(_ANGLE_GREEN_IN = new PlugIn<ValueType>(this, "AngleGreen", false, new ValueType(75,0,360)));
  addPlug(_ANGLE_BLUE_IN = new PlugIn<ValueType>(this, "AngleBlue", false, new ValueType(0,0,360)));
}

Gear_ClusteredDither::~Gear_ClusteredDither()
{
  free(_threshold);
  free(_order);
}

void Gear_ClusteredDither::internalInit()
{
  _clusterSize = CLAMP((int)_CLUSTER_SIZE_IN->type()->value(), 2, 512);
  _spotType = (eSpotType)CLAMP((int)_SPOT_TYPE_IN->type()->value(), (int)SQUARE, (int)LINE);
  _clusterWidth = _clusterSize * 3;
  _sizeX = _sizeY = 0;
  _angle[0] = DEG2RAD(CLAMP((int)_ANGLE_RED_IN->type()->value(), 0, 360));
  _angle[1] = DEG2RAD(CLAMP((int)_ANGLE_GREEN_IN->type()->value(), 0, 360));
  _angle[2] = DEG2RAD(CLAMP((int)_ANGLE_BLUE_IN->type()->value(), 0, 360));
  updateThreshold();
  updatePolarCoordinates();
  updateAngle(0);
  updateAngle(1);
  updateAngle(2);
  ASSERT_WARNING(_threshold);
  ASSERT_WARNING(_order);
}

void Gear_ClusteredDither::onUpdateSettings()
{
}

void Gear_ClusteredDither::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  
  _data = _image->data();    
  _outData = _outImage->data();
  
  int prevSizeX = _sizeX;
  int prevSizeY = _sizeY;
  _sizeX = _image->width();
  _sizeY = _image->height();
  
  NOTICE("Changing cluster");
  // If cluster size has changed, recompute threshold matrix.
  int prevClusterSize = _clusterSize;
  _clusterSize = CLAMP((int)_CLUSTER_SIZE_IN->type()->value(), 2, MAX((int)_image->height(),4));
  bool clusterChanged = (prevClusterSize != _clusterSize);
  if (clusterChanged)
    _clusterWidth = _clusterSize * 3;
  
  // XXX computeThreshold deux fois!!!
  // Set spot type.
  eSpotType prevSpotType = _spotType;
  _spotType = (eSpotType)CLAMP((int)_SPOT_TYPE_IN->type()->value(), (int)SQUARE, (int)LINE);
  bool spotTypeChanged = (prevSpotType != _spotType);
  
  if (clusterChanged || spotTypeChanged)
    updateThreshold();
  NOTICE("...done");

  if (_sizeX != prevSizeX || _sizeY != prevSizeY)
  {
    NOTICE("Updating polar coordinates");
    updatePolarCoordinates();
    NOTICE("Changing angles");
    _angle[0] = DEG2RAD(CLAMP((int)_ANGLE_RED_IN->type()->value(), 0, 360));
    _angle[1] = DEG2RAD(CLAMP((int)_ANGLE_GREEN_IN->type()->value(), 0, 360));
    _angle[2] = DEG2RAD(CLAMP((int)_ANGLE_BLUE_IN->type()->value(), 0, 360));
    
    updateAngle(0);
    updateAngle(1);
    updateAngle(2);
    NOTICE("...done");
  }
  else
  {
    NOTICE("Changing angles");
    double angleR = _angle[0];
    _angle[0] = DEG2RAD(CLAMP((int)_ANGLE_RED_IN->type()->value(), 0, 360));
    double angleG = _angle[1];
    _angle[1] = DEG2RAD(CLAMP((int)_ANGLE_GREEN_IN->type()->value(), 0, 360));
    double angleB = _angle[2];
    _angle[2] = DEG2RAD(CLAMP((int)_ANGLE_BLUE_IN->type()->value(), 0, 360));

    if (clusterChanged || _angle[0] != angleR)
      updateAngle(0);
    
    if (clusterChanged || _angle[1] != angleG)
      updateAngle(1);
    
    if (clusterChanged || _angle[2] != angleB)
      updateAngle(2);
    NOTICE("...done");
  }
  
  unsigned char *iterData    = (unsigned char*)_data;
  unsigned char *iterOutData = (unsigned char*)_outData;

  Array2DType<std::pair<int, int> >::iterator
    rIt = _rChannel[0].begin(),
    gIt = _rChannel[1].begin(),
    bIt = _rChannel[2].begin();
  
  for (int y=0; y<_sizeY; ++y)
  {
    for (int x=0; x<_sizeX; ++x, ++rIt, ++gIt, ++bIt)
    {
      // Assign RGB values.
      *iterOutData++ = getValue(*iterData++, rIt->first, rIt->second);
      *iterOutData++ = getValue(*iterData++, gIt->first, gIt->second);
      *iterOutData++ = getValue(*iterData++, bIt->first, bIt->second);
      // Skip alpha.
      iterOutData++;
      iterData++;
    }
  }  

}

void Gear_ClusteredDither::updateThreshold()
{
  int width2 = _clusterWidth*_clusterWidth;

  _threshold = (unsigned char*)realloc(_threshold, width2*sizeof(unsigned char));
  _order     = (Order*)realloc(_order, width2*sizeof(Order));

  Order *iterOrder = _order;

  // inside cell
  int i=0;
  for (int yCell=0; yCell<_clusterWidth; ++yCell)
  {
    float sy = 2*(float)yCell / (_clusterWidth-1) - 1;
    for (int xCell=0; xCell<_clusterWidth; ++xCell)
    {
      float sx = 2*(float)xCell / (_clusterWidth-1) - 1;
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
    _threshold[(iterOrder++)->first] = val / width2; // *** pas besoin de .value dans c'cas la...
}

void Gear_ClusteredDither::updatePolarCoordinates()
{
  _r.resize(_sizeX, _sizeY);
  _theta.resize(_sizeX, _sizeY);
  int maxSizeX  = _sizeX / 2;
  int maxSizeY  = _sizeY / 2;
  int minSizeX  = - (maxSizeX + (_sizeX%2));
  int minSizeY  = - (maxSizeY + (_sizeY%2));
  
  Array2DType<double>::iterator rIt = _r.begin(), thetaIt = _theta.begin();

  for (int y=minSizeY; y<maxSizeY; ++y)
    for (int x=minSizeX; x<maxSizeX; ++x)
    {
      *rIt++ = fastDist(x,y);
      *thetaIt++ = fastAngle(x,y);
    }
}

void Gear_ClusteredDither::updateAngle(int channel)
{
  ASSERT_ERROR(channel >= 0 && channel < SIZE_RGB);
  double angle = _angle[channel];

  _rChannel[channel].resize(_sizeX, _sizeY);
  
  Array2DType<double>::iterator
    rIt = _r.begin(),
    thetaIt = _theta.begin();

  Array2DType<std::pair<int, int> >::iterator
    rChannelIt = _rChannel[channel].begin();
  
  for (int y=0; y<_sizeY; ++y)
  {
    for (int x=0; x<_sizeX; ++x, ++rIt, ++thetaIt, ++rChannelIt)
    {
      double theta_c = *thetaIt + angle;
      
      int rx = (int)rint(*rIt * fastcos(theta_c) );
      int ry = (int)rint(*rIt * fastsin(theta_c) );
      
      /* Make sure rx and ry are positive and within
       * the range 0 .. width-1 (incl).  Can't use %
       * operator, since its definition on negative
       * numbers is not helpful.  Can't use ABS(),
       * since that would cause reflection about the
       * x- and y-axes.  Relies on integer division
       * rounding towards zero. */
      rx -= ((rx - isNeg(rx)*(_clusterWidth-1)) / _clusterWidth) * _clusterWidth;
      ry -= ((ry - isNeg(ry)*(_clusterWidth-1)) / _clusterWidth) * _clusterWidth;

      ASSERT_WARNING(rx >= 0 && rx <= _clusterWidth-1);
      ASSERT_WARNING(ry >= 0 && ry <= _clusterWidth-1);
      
      rChannelIt->first = rx;
      rChannelIt->second = ry;
    }
  }

}
