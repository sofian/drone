/* Gear_EncrageMask.cpp
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

#include <iostream>
#include "Gear_EncrageMask.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_EncrageMask, Gear_EncrageMask, "EncrageMask");

Gear_EncrageMask::Gear_EncrageMask(Schema *schema, std::string uniqueName) : Gear(schema, "EncrageMask", uniqueName)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_VIDEO_INK_IN = new PlugIn<VideoRGBAType>(this, "ImgInkIN"));
  addPlug(_VIDEO_DECAY_IN = new PlugIn<VideoRGBAType>(this, "ImgDecayIN"));
  
  addPlug(_INK_SPEED_IN = new PlugIn<ValueType>(this, "Ink", new ValueType(1, 0, 100)));
  addPlug(_DECAY_SPEED_IN = new PlugIn<ValueType>(this, "Decay", new ValueType(1, 0, 100)));
  addPlug(_AREA_ARRAY_IN = new PlugIn<AreaArrayType>(this, "AreaIN"));
  _imageBuffer = new VideoRGBAType();
  _imageBuffer->fill(WHITE_RGBA);
  srand(0);
}

Gear_EncrageMask::~Gear_EncrageMask()
{
  delete _imageBuffer;
}

bool Gear_EncrageMask::ready()
{
  return(_VIDEO_OUT->connected() && _VIDEO_DECAY_IN->connected() && _VIDEO_INK_IN->connected());
}

void Gear_EncrageMask::runVideo()
{
  _areaArray = _AREA_ARRAY_IN->type();

  _imageInk = _VIDEO_INK_IN->type();
  _imageDecay = _VIDEO_DECAY_IN->type();
  
  _sizeX = (int)_imageInk->width();
  _sizeY = (int)_imageInk->height();

  ASSERT_ERROR(_imageInk->width() == _imageDecay->width() && _imageInk->height() == _imageDecay->height());
  
  _inkSpeed = _INK_SPEED_IN->type()->value();
  _decaySpeed = _DECAY_SPEED_IN->type()->value();
  
  //  NOTICE("Size = %d x %d", _sizeX, _sizeY);
  
  _imageOut = _VIDEO_OUT->type();
  if (_sizeX != (int)_imageBuffer->width() || _sizeY != (int)_imageBuffer->height())
  {
    NOTICE("Reset buffer image %d x %d", _sizeX, _sizeY);
    _imageBuffer->resize((size_t)_sizeX, (size_t)_sizeY);
    _imageBuffer->fill(WHITE_RGBA);
  }

  _size = (int)_imageBuffer->size();

  _imageOut->resize((size_t)_sizeX, (size_t)_sizeY);

  // Decay image.
  int nDecayPix = (int)(_decaySpeed * _imageBuffer->size());
  for (int i=0; i<nDecayPix; ++i)
  {
    int pos = rand() % _size;
    _imageBuffer->operator[](pos) = _imageDecay->operator[](pos);
  }

  // Ink image.
  for (AreaArrayType::const_iterator it = _areaArray->begin(); it != _areaArray->end(); ++it)
  {
    ASSERT_ERROR(it->x0 <= it->x1 && it->y0 <= it->y1);
    int minX = MAX((int)it->x0,0);
    int maxX = MIN((int)it->x1,_sizeX-1);
    int minY = MAX((int)it->y0,0);
    int maxY = MIN((int)it->y1,_sizeY-1);
    size_t rowSize = maxX-minX;
    int size = (maxX-minX)*(maxY-minY);
    int nInkPix = (int)(_inkSpeed * size);
    for (int i = 0; i < nInkPix; ++i)
    {
      int pos = rand() % size;
      int posY = minY + pos / rowSize;
      int posX = minX + pos % rowSize;
      _imageBuffer->operator()(posX, posY) = _imageInk->operator()(posX, posY);
    }
  }

  RGBA *_iterBuffer = _imageBuffer->data();
  RGBA *_iterDecay = _imageDecay->data();
  RGBA *_iterOut = _imageOut->data();
  for (int p = 0; p < _size; ++p)
  {
    if (memcmp(_iterBuffer, &WHITE_RGBA, sizeof(RGBA)) == 0)
      *_iterOut = *_iterDecay;
    else
      *_iterOut = *_iterBuffer;
    ++_iterBuffer;
    ++_iterDecay;
    ++_iterOut;
  }
}


