/* Gear_DiffDist.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#include "Gear_DiffDist.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_DiffDist, Gear_DiffDist, "DiffDist")

unsigned long xcng(void){  // takes 60 nanosecs, passes all tests
  static unsigned long x=123456789,y=521288629,z,w;
  z=x+y; x=y;  w=(z<<8); y=w+17*(z>>24);
  if (z<x) y=y+4352;
  if (y<w) y=y+17;
  return( y );}

Gear_DiffDist::Gear_DiffDist(Schema *schema, QString uniqueName) : Gear(schema, "DiffDist", uniqueName)
{
  addPlug(_FACTOR_IN = new PlugIn<ValueType>(this, "Factor"));
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
}

Gear_DiffDist::~Gear_DiffDist()
{

}

bool Gear_DiffDist::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}

void Gear_DiffDist::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  if (_imageA->isNull())
    return;

  _imageB = _VIDEO_IN_B->type();    
  if (_imageB->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  float factor = _FACTOR_IN->type()->value();


  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _imageOutSizeX = MAX(_imageASizeX, _imageBSizeX);
  _imageOutSizeY = MAX(_imageASizeY, _imageBSizeY);
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);
  
  if (factor <= 0)
  {
     memcpy(_outImage->data(), _imageA->data(), _imageA->size()*sizeof(RGBA));
     return;
  }
   
  
  memset(_outImage->data(), 0, _outImage->size()*sizeof(RGBA));

  _dataA = _imageA->data();    
  _dataB = _imageB->data();    
  _outData = _outImage->data();

  int colorIntensity=0;
  int destX=0;
  int destY=0;
  int patchY=0;



  for (int y=0;y<_iterSizeY;y++)
    for (int x=0;x<_iterSizeX;x++)
    {

      colorIntensity = (_dataB[y*_imageBSizeX+x].r + _dataB[y*_imageBSizeX+x].g + _dataB[y*_imageBSizeX+x].b) / 3;            
      colorIntensity *= (int)fabs(factor);


      destX = x + colorIntensity;

      destY = y + colorIntensity;


      if (destX < 0)
        destX=0;

      if (destY < 0)
        destY=0;

      if (destX > _imageOutSizeX)
        destX = _imageOutSizeX;

      if (destY > _imageOutSizeY)
        destY = _imageOutSizeY;

      _outData[destY*_imageOutSizeX+destX].r = _dataA[y*_imageASizeX+x].r;
      _outData[destY*_imageOutSizeX+destX].g = _dataA[y*_imageASizeX+x].g;
      _outData[destY*_imageOutSizeX+destX].b = _dataA[y*_imageASizeX+x].b;

      if (destY != y)
      {
        patchY = y - colorIntensity;
        if (patchY < 0)
          patchY=0;


        _outData[y*_imageOutSizeX+x].r = _dataA[patchY*_imageASizeX+x].r;
        _outData[y*_imageOutSizeX+x].g = _dataA[patchY*_imageASizeX+x].g;
        _outData[y*_imageOutSizeX+x].b = _dataA[patchY*_imageASizeX+x].b;
      }

    }

}

