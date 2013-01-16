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
#include "DroneMath.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear()
{
  return new Gear_Sobel();
}


}

Gear_Sobel::Gear_Sobel() : Gear("Sobel")
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  NOTICE("For now, this edger is imperfect since it doesn't iterates through all points for efficiency reasons.");
}

Gear_Sobel::~Gear_Sobel()
{

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

  // Process center
  for (int y=0;y<_sizeY;++y)
  {
    // Set row iterators.
    _p1 = (unsigned char*) (_image->row(MAX(0,y-1))    );
    _p2 = (unsigned char*) (_image->row(MAX(0,y-1))    );
    _p3 = (unsigned char*) (_image->row(MAX(0,y-1)) + 1);
    _p4 = (unsigned char*) (_image->row(y)      );
    _p6 = (unsigned char*) (_image->row(y)   + 1);
    _p7 = (unsigned char*) (_image->row(MIN(_sizeY-1,y+1))    );
    _p8 = (unsigned char*) (_image->row(MIN(_sizeY-1,y+1))    );
    _p9 = (unsigned char*) (_image->row(MIN(_sizeY-1,y+1)) + 1);
    
    _iterOutData = (unsigned char*) _outImage->row(y);
    
    // Process leftmost point.
    singleStep();

    // Reset kernel's leftmost pointers.
    _p1 -= SIZE_RGBA;
    _p4 -= SIZE_RGBA;
    _p7 -= SIZE_RGBA;
    
    // Process center.
    for (int x=1;x<_sizeX-1;++x)
      singleStep();

    // Reset kernel's rightmost pointers.
    _p3 -= SIZE_RGBA;
    _p6 -= SIZE_RGBA;
    _p9 -= SIZE_RGBA;

    // Process rightmost point.
    singleStep();

  }

}

