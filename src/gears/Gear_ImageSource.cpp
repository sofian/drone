/* Gear_ImageSource.cpp
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
#include "Gear_ImageSource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_ImageSource, Gear_ImageSource, "ImageSource");

const std::string Gear_ImageSource::SETTING_FILENAME = "Filename";

Gear_ImageSource::Gear_ImageSource(Engine *engine, std::string name) : 
  Gear(engine, "ImageSource", name),
  _sizeX(0),
  _sizeY(0)
{
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");
}

Gear_ImageSource::~Gear_ImageSource()
{
}

bool Gear_ImageSource::ready()
{
  return(_VIDEO_OUT->connected());
}

void Gear_ImageSource::onUpdateSettings()
{
  char tempstr[1024];

  strcpy(tempstr,_settings.get(SETTING_FILENAME)->valueStr().c_str());

  std::cout << "opening file : " << tempstr << std::endl;

  QImage image(tempstr);

  _sizeX = image.width();
  _sizeY = image.height();

  std::cout << "image size X : " << _sizeX << std::endl;
  std::cout << "image size Y : " << _sizeY << std::endl;

  // Fill buffer with image.
  _imageBuffer.resize(_sizeX, _sizeY);
  for (int y = 0; y < _sizeY; ++y)
    for (int x = 0; x < _sizeX; ++x)
    {
      QRgb pix = image.pixel(x,y);
      _imageBuffer(x,y) = RGBA( qRed(pix), qGreen(pix), qBlue(pix), qAlpha(pix) );
    }
  
}

void Gear_ImageSource::runVideo()
{
  _VIDEO_OUT->type()->resize(_sizeX, _sizeY);

  memcpy(_VIDEO_OUT->type()->data(), _imageBuffer.data(), _imageBuffer.size()*sizeof(RGBA));
}



