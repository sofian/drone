/* Gear_AreaVideoSelect.cpp
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
#include "Gear_AreaVideoSelect.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaVideoSelect(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaVideoSelect";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaVideoSelect::Gear_AreaVideoSelect(Schema *schema, std::string uniqueName) : Gear(schema, "AreaVideoSelect", uniqueName)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoOut", true));
  addPlug(_SELECT_IN = new PlugIn<AreaType>(this, "Select", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "Video", true));

  _rasterer = new Rasterer();
}

Gear_AreaVideoSelect::~Gear_AreaVideoSelect()
{
  delete _rasterer;
}

void Gear_AreaVideoSelect::runVideo()
{
  _area = _SELECT_IN->type();
  _imageIn = _VIDEO_IN->type();
  _imageOut = _VIDEO_OUT->type();

  _imageOut->resize(_area->width(), _area->height());

  _rasterer->setImage(_imageOut);
  _rasterer->setColor(CLEAR_RGBA);

  //  ASSERT_ERROR(0 <= _area->x0() && _area->x0() + _area->width() <= _imageIn->width() &&
  //               0 <= _area->y0() && _area->y0() + _area->height() <= _imageIn->height());
  //  NOTICE("Area in: %d %d %d %d", _area->x0(), _area->y0(),_area->width(), _area->height());  
  //  NOTICE("Image out: %d %d ", _imageOut->width(), _imageOut->height());  
  
  size_t rowWidth = (int)_area->width() * sizeof(RGBA);

  RGBA val;

//   bool clipTop    = (_area->y0() < 0);
//   bool clipBottom = (_area->y0() + _area->height() > _imageIn->height());
//   bool clipLeft   = (_area->x0() < 0);
//   bool clipRight  = (_area->x0() + _area->width() > _imageIn->width());

//   //  VideoRGBAType::iterator it = _imageOut->begin();

//   int colOut = 0, rowOut = 0;
//   int colIn = 0,  rowIn = 0;
  
//   // Parse rows that are over (outside) the image.
//   if (clipTop)
//   {
//     rowOut += MAX((size_t)(-_area->y0()), _imageOut->height());
//     std::fill(_imageOut->begin(),
//               _imageOut->begin() + rowOut * _imageOut->width(),
//               RGBA(255,0,0,0));
//     //              CLEAR_RGBA);
//   }
//   else
//     rowIn = MIN(_area->y0(), (int)_imageIn->height());

// //   if (clipLeft)
// //   {
// //     colOut = -_area->x0();
// //     _rasterer->rect(0,rowOut,colOut,_imageOut->height()-1,true);
// //   }

// //   if (clipRight)
// //   {
// //     size_t width = MAX(0, (int)(_area->x0() + _area->width() - _imageIn->width()));
// //     size_t rowWidth = width * sizeof(RGBA);
// //     for (int yIn = rowIn, yOut = rowOut ; yIn < _imageIn->height() && yOut < _imageOut->height(); ++yIn, ++yOut)
// //       memcpy(&_imageOut->operator()(colOut,yOut),
// //              &_imageIn->operator()(colIn,yIn),
// //              rowWidth);
// //     _rasterer->rect(colOut+width,rowOut,_imageOut->width()-colOut-width,rowOut+_imageIn->height()-rowIn,true);
// //   }
// //   else
// //   {
// //     size_t rowWidth = _imageOut->width() * sizeof(RGBA);
// //     for ( ; rowIn < _imageIn->height() && rowOut < _imageOut->height(); ++rowIn, ++rowOut)
// //       memcpy(&_imageOut->operator()(colOut,rowOut),
// //              &_imageIn->operator()(colIn,rowIn),
// //              rowWidth);
// //   }        

  
  for (int j=0, y=_area->y0(); j<(int)_area->height(); ++j, ++y)
  {
    if (0 <= y && y < _imageIn->height()) // inside image
    {
      for (int i=0, x=_area->x0(); i<(int)_area->width(); ++i, ++x)
      {
        if (0 <= x && x < _imageIn->width()) // inside image
          _imageOut->operator()(i,j) = _imageIn->get(x,y);
        else
          _imageOut->operator()(i,j) = CLEAR_RGBA;
      }
    }
    else
      std::fill(_imageOut->row(j), _imageOut->row(j) + _imageOut->width(), CLEAR_RGBA);
  }

//   // Parse rows that are under (outside) the image.
//   if (clipBottom)
//     std::fill(_imageOut->begin()  + _area->width() * MAX(0, (int)(_area->y0() + _area->height() - _imageIn->height())),
//               _imageOut->end(),
//               RGBA(255,0,0,0));
//   //              CLEAR_RGBA);

//   // Parse rows that are in the middle of (inside) the image.
//   for (int j=0, y=_area->y0(); j<(int)_area->height(); ++j, ++y)
//   {
//     if (0 <= y && y < _imageIn->height()) // inside image
//     {
//       for (int i=0, x=_area->x0(); i<(int)_area->width(); ++i, ++x)
//       {
//         if (0 <= x && x < _imageIn->width()) // inside image
//           val = _imageIn->get(x,y);
//         else
//           val = CLEAR_RGBA;
//         _imageOut->operator()(i,j) = val;
//       }
//     }
//     else
//       for (int i=0; i<(int)_area->width(); ++i)
//         _imageOut->operator()(i,j) = CLEAR_RGBA;
//   }

//   // Parse rows that are under (outside) the image.
//   for (int j=0, y=_area->y0(); j<(int)_area->height(); ++j, ++y)
//   {
//     if (0 <= y && y < _imageIn->height()) // inside image
//     {
//       for (int i=0, x=_area->x0(); i<(int)_area->width(); ++i, ++x)
//       {
//         if (0 <= x && x < _imageIn->width()) // inside image
//           val = _imageIn->get(x,y);
//         else
//           val = CLEAR_RGBA;
//         _imageOut->operator()(i,j) = val;
//       }
//     }
//     else
//       for (int i=0; i<(int)_area->width(); ++i)
//         _imageOut->operator()(i,j) = CLEAR_RGBA;
//   }

  //    memcpy(_imageOut->row(i), &_imageIn->operator()(x,y), rowWidth);


//   _area = _SELECT_IN->type();
//   _imageIn = _VIDEO_IN->type();
//   _imageOut = _VIDEO_OUT->type();

//   _imageOut->resize(_area->width(), _area->height());
//   ASSERT_ERROR(0 <= _area->x0() && _area->x0() + _area->width() <= _imageIn->width() &&
//                0 <= _area->y0() && _area->y0() + _area->height() <= _imageIn->height());
//   //  NOTICE("Area in: %d %d %d %d", _area->x0(), _area->y0(),_area->width(), _area->height());  
//   //  NOTICE("Image out: %d %d ", _imageOut->width(), _imageOut->height());  
  
//   int x = _area->x0();
//   size_t rowWidth = (int)_area->width() * sizeof(RGBA);

//   for (int i=0, y=_area->y0(); i<(int)_area->height(); ++i, ++y)
//     memcpy(_imageOut->row(i), &_imageIn->operator()(x,y), rowWidth);
}
