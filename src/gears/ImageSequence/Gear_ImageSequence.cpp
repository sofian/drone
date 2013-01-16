/* Gear_ImageSequence.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
 * Modified by Mathieu Petit-Clair
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
#include <stdio.h>

#include "Gear_ImageSequence.h"
#include "Engine.h"

#include "GearMaker.h"
#include "DroneMath.h"

const QString Gear_ImageSequence::SETTING_FILENAME = "Filename";

extern "C" {
Gear* makeGear()
{
  return new Gear_ImageSequence();
}


}

Gear_ImageSequence::Gear_ImageSequence() : 
  Gear("ImageSequence"),
  _current(0),
  _nFramesCurrent(0)
{
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", true));
  addPlug(_SPEED_IN = new PlugIn<ValueType>(this, "Speed", false, new ValueType(1, 0, 1)));

  EnumType *orderingFunc = new EnumType(N_ORDERING_TYPES, SHUFFLE);
  orderingFunc->setLabel(SHUFFLE, "Shuffle"); 
  orderingFunc->setLabel(SEQUENCE, "Sequence");
  addPlug(_ORDERING_IN =  new PlugIn<EnumType>(this, "ordType", false, orderingFunc));

  _settings.add(Property::FILENAMES, SETTING_FILENAME)->valueStr("");
}

Gear_ImageSequence::~Gear_ImageSequence()
{
}

void Gear_ImageSequence::onUpdateSettings()
{
  _imageBuffers.clear();
    
  std::vector<QString> filenames = _settings.get(SETTING_FILENAME)->valueStrList();
  
  for (int i=0; i<filenames.size(); ++i)
  {
    std::cout << filenames[i] << std::endl;
    Array2D<RGBA> img;
    loadImage(filenames[i], img);
    _imageBuffers.push_back( img );
  }
  
}

void Gear_ImageSequence::internalInit()
{
  _current = 0;
  _nFramesCurrent = 0;
}

void Gear_ImageSequence::loadImage(const QString& filename, Array2D<RGBA>& image)
{
  QImage img(filename.c_str());
    
  // Fill buffer with image.
  image.resize(img.width(), img.height());
  
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x)
    {
      QRgb pix = img.pixel(x,y);
      image(x,y) = RGBA( qRed(pix), qGreen(pix), qBlue(pix), qAlpha(pix) );
    }
}

void Gear_ImageSequence::runVideo()
{
  int maxFrames = (int) (1.0f / CLAMP(_SPEED_IN->type()->value(), 0.0333f, 1.0f));
  if (_nFramesCurrent >= maxFrames)
  {
    _orderingType = CLAMP((eOrderingType)_ORDERING_IN->type()->value(), SHUFFLE, SEQUENCE);
    switch(_orderingType)
    {
    case SHUFFLE :
      _current = rand() % _imageBuffers.size();
      break;
    case SEQUENCE :
      ++_current %= _imageBuffers.size();
      break;
    }
    _nFramesCurrent = 0;
  }
  Array2D<RGBA> *current = &_imageBuffers[_current];
  _VIDEO_OUT->type()->resize(current->width(), current->height());
  memcpy(_VIDEO_OUT->type()->data(), current->data(), current->size() * sizeof(RGBA));

  _nFramesCurrent++;
}



