/* Gear_DummySource.cpp
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
#include "Gear_DummySource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_DummySource, Gear_DummySource, "DummySource");

Gear_DummySource::Gear_DummySource(Schema *schema, std::string uniqueName) : Gear(schema, "DummySource", uniqueName)
{    
  addPlug(_X_IN = new PlugIn<ValueType>(this, "X", new ValueType(160, 0, 320)));
  addPlug(_Y_IN = new PlugIn<ValueType>(this, "Y", new ValueType(120, 0, 240)));
  addPlug(_INTENSITY_IN = new PlugIn<ValueType>(this, "Int", new ValueType(127, 0, 255)));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  _rast = new Rasterer();
}

Gear_DummySource::~Gear_DummySource()
{
  delete _rast;
}

bool Gear_DummySource::ready()
{
  return(_VIDEO_OUT->connected());
}

void Gear_DummySource::onUpdateSettings()
{
}

void Gear_DummySource::runVideo()
{
  unsigned char intensity = (unsigned char)CLAMP(_INTENSITY_IN->type()->value(), 0.0f, 255.0f);
  RGBA fg(intensity, intensity, intensity, 0);
  //RGBA fg(255,0,0,0);
  int x = (int)_X_IN->type()->value();
  int y = (int)_Y_IN->type()->value();

  int x0 = CLAMP(x - 30, 0, 319);
  int x1 = CLAMP(x + 30, 0, 319);
  int y0 = CLAMP(y - 30, 0, 239);
  int y1 = CLAMP(y + 30, 0, 239);
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(320,240);
  _outImage->fill(BLACK_RGBA);

  _rast->setColor(fg);
  _rast->setImage(_outImage);
  _rast->rect(x0, y0, x1, y1, true);
}

void Gear_DummySource::runAudio()
{
  //  _audioBuffer = _AUDIO_OUT->type()->type()->data();

  //mpeg3_read_audio(_file, signal, NULL, 1, 128, 0);
}


