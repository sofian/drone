/* Gear_AsciiArt.cpp
 * Copyright (C) 2004 Mathieu Petit-Clair
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

#include "Gear_AsciiArt.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_AsciiArt, Gear_AsciiArt, "AsciiArt")

Gear_AsciiArt::Gear_AsciiArt(Schema *schema, std::string uniqueName) : Gear(schema, "AsciiArt", uniqueName)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("Amount", 1.0f);
}

Gear_AsciiArt::~Gear_AsciiArt()
{

}

bool Gear_AsciiArt::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}



void Gear_AsciiArt::runVideo()
{
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->width(), _image->height());
  _data = (unsigned char*)_image->_data;
  _outData = (unsigned char*)_outImage->_data;
  float tresh = _AMOUNT_IN[0];

  _sizeX = _image->width();
  _sizeY = _image->height();

  int pp=0;
  for (int y=1;y<_sizeY-2;y++)
  {
    for (int x=1;x<_sizeX-2;x++)
    {
      for (int z=0;z<4;z++)
      {
        _outData[y*_sizeX*4+x*4+z] = (4* _data[y*_sizeX*4+x*4+z] - 
                                      _data[(y-1)*_sizeX*4+x*4+z] -
                                      _data[(y+1)*_sizeX*4+x*4+z] -
                                      _data[y*_sizeX*4+(x-1)*4+z] -
                                      _data[y*_sizeX*4+(x+1)*4+z])/2;

        if (_outData[y*_sizeX*4+x*4+z]<tresh)
          _outData[y*_sizeX*4+x*4+z]=0;

      }
    }
  }
}
