/* Gear_AlphaMix.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#include "Gear_AlphaMix.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AlphaMix(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AlphaMix";
  gearInfo.classification = GearClassifications::video().composition().instance();
  return gearInfo;
}
}

Gear_AlphaMix::Gear_AlphaMix(Schema *schema, std::string uniqueName) : Gear(schema, "AlphaMix", uniqueName)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));

  EnumType *mixFunc = new EnumType(N_ALPHAMIX_TYPES, OVER);
  mixFunc->setLabel(OVER,"Over");
  mixFunc->setLabel(IN,"In");
  mixFunc->setLabel(OUT,"Out");
  mixFunc->setLabel(ATOP,"Atop");
  mixFunc->setLabel(XOR,"Xor");
  addPlug(_MIXFUNC_IN = new PlugIn<EnumType>(this, "MixFunc", mixFunc));
}

Gear_AlphaMix::~Gear_AlphaMix()
{

}

bool Gear_AlphaMix::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}

void Gear_AlphaMix::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  _imageB = _VIDEO_IN_B->type();
  _outImage = _VIDEO_OUT->type();

  _imageOutSizeX = MAX(_imageA->width(), _imageB->width());
  _imageOutSizeY = MAX(_imageA->height(), _imageB->height());
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);

  if (_imageA->isNull())
  {
    if (_imageB->isNull())
      return;
      
    memcpy(_outImage->data(), _imageB->data(), _imageA->size()*sizeof(RGBA));
    return;
  }
  else if (_imageB->isNull())
  {  
    memcpy(_outImage->data(), _imageA->data(), _imageA->size()*sizeof(RGBA));
    return;
  }

  _mixType = CLAMP((eAlphaMixType)_MIXFUNC_IN->type()->value(), OVER, XOR);

  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _imageInA = (unsigned char *)_imageA->data();
  _imageInB = (unsigned char *)_imageB->data();
  _imageOut = (unsigned char *)_outImage->data();
  
  switch (_mixType)
  {
  case OVER:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_over(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case IN:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_in(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case OUT:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_out(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case ATOP:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_atop(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case XOR:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_xor(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
 default:;
   memcpy(_imageOut, _imageInA, _outImage->size() * sizeof(RGBA));
  }

}

