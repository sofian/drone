/* Gear_Saturation.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#include "Gear_Saturation.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_Saturation, Gear_Saturation, "Saturation")

Gear_Saturation::Gear_Saturation(Engine *engine, std::string name) : Gear(engine, "Saturation", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(1.0f, 0.0f, 10.0f)));

}

Gear_Saturation::~Gear_Saturation()
{

}

bool Gear_Saturation::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Saturation::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _size = (int) _image->size();
  
  float amount = _AMOUNT_IN->type()->value();
    
  _imageIn  = (const unsigned char*) _image->data();
  _imageOut = (unsigned char*) _outImage->data();

  float oneMinusAmount = 1.0f-amount;
  int rwgt = (int)(19595 * oneMinusAmount);
  int gwgt = (int)(38470 * oneMinusAmount);
  int bwgt = (int)(7471  * oneMinusAmount);

  int bw=0;

  if (0 <= amount && amount <=1) // optimisation: no clamping needed
  {
    for (int p=0; p<_size; ++p)
    {
      _r = _imageIn[0];
      _g = _imageIn[1];
      _b = _imageIn[2];
      
      bw = (_r*rwgt + _g*gwgt + _b*bwgt) >> 16;
      
      _imageOut[0] = (unsigned char) (bw + _r*amount);
      _imageOut[1] = (unsigned char) (bw + _g*amount);
      _imageOut[2] = (unsigned char) (bw + _b*amount);
      
      _imageIn+=SIZE_RGBA;
      _imageOut+=SIZE_RGBA;
    }
  }
  else
  {
    for (int p=0; p<_size; ++p)
    {
      _r = _imageIn[0];
      _g = _imageIn[1];
      _b = _imageIn[2];
    
      bw = (_r*rwgt + _g*gwgt + _b*bwgt) >> 16;
    
      _r = (int) (bw + _r*amount);
      _g = (int) (bw + _g*amount);
      _b = (int) (bw + _b*amount);
    
      _imageOut[0] = (unsigned char) CLAMP0255(_r);
      _imageOut[1] = (unsigned char) CLAMP0255(_g);
      _imageOut[2] = (unsigned char) CLAMP0255(_b);
    
      _imageIn+=SIZE_RGBA;
      _imageOut+=SIZE_RGBA;
    }
  }

}

