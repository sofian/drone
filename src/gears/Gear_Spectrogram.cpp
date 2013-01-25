/* Gear_Spectrogram.cpp
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

#include "Gear_Spectrogram.h"
#include "Engine.h"
#include <iostream>


Register_Gear(MAKERGear_Spectrogram, Gear_Spectrogram, "Spectrogram")

Gear_Spectrogram::Gear_Spectrogram() : Gear("Spectrogram")
{
  addPlug(_FFT_IN = new PlugIn<FFTType>(this, "In", true));
  addPlug(_SIZE_X = new PlugIn<ValueType>(this, "SizeX", false, new ValueType(512,32,768)));
  addPlug(_SIZE_Y = new PlugIn<ValueType>(this, "SizeY", false, new ValueType(150,32,768)));

  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "Out", true));
  
  _rasterer = new Rasterer();
}

Gear_Spectrogram::~Gear_Spectrogram()
{
}

void Gear_Spectrogram::runVideo()
{
  int sizeY = CLAMP((int)_SIZE_Y->type()->value(),32,512);
  int sizeX = CLAMP((int)_SIZE_X->type()->value(),32,1440);
  
  int spectrumSize = _FFT_IN->type()->spectrumSize();
  
  float rectSizeX = (float)sizeX / (float)(spectrumSize);  
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(sizeX,sizeY);
  _outData = _outImage->data();

  _outImage->fill(BLACK_RGBA);

  _rasterer->setImage(_outImage);
  _rasterer->setColor(255,255,255);
  

  int rectSizeY;
  for (int i=0;i<spectrumSize;++i)
  {                                           
    //std::cout << _FFT_IN->type()->power(i) << std::endl;
    //_rasterer->setColor(_FFT_IN->type()->power(i),_FFT_ IN->type()->power(i),_FFT_IN->type()->power(i));
    rectSizeY =  _FFT_IN->type()->power(i) > sizeY ? sizeY : _FFT_IN->type()->power(i);
    _rasterer->rect(i*rectSizeX, sizeY-rectSizeY, i*rectSizeX+rectSizeX, sizeY, true);  
  }
}



