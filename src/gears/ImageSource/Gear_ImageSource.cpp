/* Gear_ImageSource.cpp
 * Copyright (C) 2004--2005 Jean-Sebastien Senecal
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

#include "Gear_ImageSource.h"
#include "Engine.h"

#include "GearMaker.h"
#include "DroneMath.h"

const std::string Gear_ImageSource::SETTING_FILENAME = "Filename";

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_ImageSource(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ImageSource";
  gearInfo.classification = GearClassifications::video().IO().instance();
  return gearInfo;
}
}

Gear_ImageSource::Gear_ImageSource(Schema *schema, std::string uniqueName) : 
  Gear(schema, "ImageSource", uniqueName),
  _current(0)
{
  // Inputs.
  addPlug(_FRAME_IN = new PlugIn<ValueType>(this, "Frame", false, new ValueType(0, 0, 0)));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_N_FRAMES_OUT = new PlugOut<ValueType>(this, "NFrames", false));

  _settings.add(Property::FILENAMES, SETTING_FILENAME)->valueStr("");
}

Gear_ImageSource::~Gear_ImageSource()
{
}

void Gear_ImageSource::onUpdateSettings()
{
  _imageBuffers.clear();
    
  std::vector<std::string> filenames = _settings.get(SETTING_FILENAME)->valueStrList();
  
  for (int i=0; i<(int)filenames.size(); ++i)
  {
    std::cout << filenames[i] << std::endl;
    Array2D<RGBA> img;
    loadImage(filenames[i], img);
    _imageBuffers.push_back( img );
  }
  
}

void Gear_ImageSource::loadImage(const std::string& filename, Array2D<RGBA>& image)
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

void Gear_ImageSource::runVideo()
{
  //  int maxFrames = (int) (1.0f / CLAMP(_SPEED_IN->type()->value(), 0.0333f, 1.0f));
  //if (_nFrames >= maxFrames)
  //{
  //  _current = rand() % _imageBuffers.size();
  //  _nFrames = 0;
  //}
  _current = &_imageBuffers[CLAMP(_FRAME_IN->type()->intValue(), 0, (int)_imageBuffers.size()-1)];
  _VIDEO_OUT->type()->resize(_current->width(), _current->height());
  std::copy(_current->begin(), _current->end(), _VIDEO_OUT->type()->begin());

  // Output number of frames.
  _N_FRAMES_OUT->type()->setValue(_imageBuffers.size());
}



