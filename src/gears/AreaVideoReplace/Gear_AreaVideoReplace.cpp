/* Gear_AreaVideoReplace.cpp
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

#include <iostream>
#include "Gear_AreaVideoReplace.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear()
{
  return new Gear_AreaVideoReplace();
}


}

Gear_AreaVideoReplace::Gear_AreaVideoReplace() : Gear("AreaVideoReplace")
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoOut", true));

  addPlug(_REPLACE_AREA_IN = new PlugIn<AreaType>(this, "ReplaceArea", true));
  addPlug(_REPLACE_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ReplaceVideo", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "VideoIn", true));

  _imageInBuffer = new VideoRGBAType();
  _imageReplaceInBuffer = new VideoRGBAType();
}

Gear_AreaVideoReplace::~Gear_AreaVideoReplace()
{
  delete _imageInBuffer;
  delete _imageReplaceInBuffer;
}

void Gear_AreaVideoReplace::runVideo()
{
  _replaceArea = _REPLACE_AREA_IN->type();
  _imageReplaceIn = _REPLACE_VIDEO_IN->type();
  _imageIn = _VIDEO_IN->type();
  _imageOut = _VIDEO_OUT->type();
  
  _imageInBuffer->resize(_imageIn->width(), _imageIn->height());
  _imageReplaceInBuffer->resize(_imageReplaceIn->width(), _imageReplaceIn->height());

  memcpy(_imageInBuffer->data(), _imageIn->data(), _imageIn->size() * sizeof(RGBA));
  memcpy(_imageReplaceInBuffer->data(), _imageReplaceIn->data(), _imageReplaceIn->size() * sizeof(RGBA));

  _imageInBuffer->setIsAlphaPremultiplied(false);
  _imageReplaceInBuffer->setIsAlphaPremultiplied(false);
  _imageInBuffer->premultiplyAlpha();
  _imageReplaceInBuffer->premultiplyAlpha();
  
  _imageOut->resize(_imageIn->width(), _imageIn->height());
  
  //  ASSERT_ERROR(0 <= _replaceArea->x0() && _replaceArea->x0() + _replaceArea->width() <= _imageIn->width() &&
  //               0 <= _replaceArea->y0() && _replaceArea->y0() + _replaceArea->height() <= _imageIn->height());
  //  NOTICE("replace area: %d %d %d %d",  _replaceArea->x0(), _replaceArea->y0(),_replaceArea->width(), _replaceArea->height());  
  //  NOTICE("Replace image in: %d %d ", _imageReplaceIn->width(), _imageReplaceIn->height());  
  ASSERT_ERROR(_replaceArea->width() == _imageReplaceIn->width() && _replaceArea->height() == _imageReplaceIn->height());

//   int x0;
//   size_t rowWidth
//   if (_replaceArea->x0() >= 0)
//   {
//     x0 = _replaceArea->x0();
//     rowWidth = (int)_replaceArea->width() * sizeof(RGBA);
//   }
//   else
//   {
//     x0 = 0;
//     rowWidth = (int)_replaceArea->width() * sizeof(RGBA);
//   }

  // Copy everything.
  memcpy(_imageOut->data(), _imageIn->data(), _imageIn->size() * sizeof(RGBA));

  // Copy the subwindow.
  for (int j=0, y=_replaceArea->y0(); j<(int)_replaceArea->height(); ++j, ++y)
  {
     if (0 <= y && y < _imageIn->height()) // inside image
     {
       for (int i=0, x=_replaceArea->x0(); i<(int)_replaceArea->width(); ++i, ++x)
       {
         if (0 <= x && x < _imageOut->width()) // inside image
           alpha_over((unsigned char*)&_imageOut->operator()(x,y),
                      (const unsigned char*)&_imageReplaceIn->operator()(i,j),
                      (const unsigned char*)&_imageIn->operator()(x,y), SIZE_RGBA);
       }
     }
  }

  _imageOut->demultiplyAlpha();
  //  for (int i=0, y=_replaceArea->y0(); i<(int)_replaceArea->height(); ++i, ++y)
  //    memcpy(&_imageOut->operator()(x,y), _imageReplaceIn->row(i), rowWidth);
}
