/* Gear_Sobel.cpp
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

#include "Gear_Sobel.h"                       
#include "Engine.h"
#include "Math.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Sobel(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Sobel";
  gearInfo.classification = GearClassifications::video().featureExtraction().instance();
  return gearInfo;
}
}

Gear_Sobel::Gear_Sobel(Schema *schema, std::string uniqueName) : Gear(schema, "Sobel", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  NOTICE("For now, this edger is imperfect since it doesn't iterates through all points for efficiency reasons.");
}

Gear_Sobel::~Gear_Sobel()
{

}

bool Gear_Sobel::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Sobel::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _data = (unsigned char*)_image->data();
  _outData = (unsigned char*)_outImage->data();

  _sizeX = _image->width();
  _sizeY = _image->height();

  // XXX temporary hack (since we don't process the borders for now)
  std::copy(_image->data(), _image->data() + _image->size(), _outImage->data());

  for (int y=1;y<_sizeY-1;y++)
  {
    // Set row iterators.
    _p1 = (unsigned char*) (_image->row(y-1)    );
    _p2 = (unsigned char*) (_image->row(y-1) + 1);
    _p3 = (unsigned char*) (_image->row(y-1) + 2);
    _p4 = (unsigned char*) (_image->row(y)      );
    _p6 = (unsigned char*) (_image->row(y)   + 2);
    _p7 = (unsigned char*) (_image->row(y+1)    );
    _p8 = (unsigned char*) (_image->row(y+1) + 1);
    _p9 = (unsigned char*) (_image->row(y+1) + 2);
    
    _iterOutData = (unsigned char*) (_outImage->row(y) + 1); 

    for (int x=1;x<_sizeX-1;x++)
    {
      for (int z=0; z<SIZE_RGBA; ++z)
      {
        // Compute the kernel function.
        *_iterOutData++ = CLAMP0255(fastabs(*_p1 + (*_p2 << 1) + *_p3 - *_p7 - (*_p8 << 1) - *_p9) +
                                    fastabs(*_p3 + (*_p6 << 1) + *_p9 - *_p1 - (*_p4 << 1) - *_p7));
        
        // Update iterators.
        _p1++; _p2++; _p3++; _p4++; _p6++; _p7++; _p8++; _p9++;
      }
    }

  }

}

