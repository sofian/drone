/* Gear_AsciiArt.cpp
 * Copyright (C) 2004 Mathieu Petit-Clair
 * Some code inspired from other gears by Jean-Sebastien Senecal
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

#include "Gear_AsciiArt.h"                       
#include "Engine.h"

#include "GearMaker.h"
#include "Math.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AsciiArt(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AsciiArt";
  gearInfo.classification = GearClassifications::video().distortion().instance();
  return gearInfo;
}
}

Gear_AsciiArt::Gear_AsciiArt(Schema *schema, std::string uniqueName) : 
  Gear(schema, "AsciiArt", uniqueName)
{
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIn"));

  EnumType *renderingFunc = new EnumType(N_RENDERING_TYPES, FAST);
  renderingFunc->setLabel(FAST, "Fast"); 
  renderingFunc->setLabel(GOOD, "Good");
  addPlug(_RENDER_IN =  new PlugIn<EnumType>(this, "Render", renderingFunc));

  EnumType *ditheringFunc = new EnumType(N_DITHERING_TYPES, NONE);
  ditheringFunc->setLabel(NONE, "None"); 
  ditheringFunc->setLabel(ERROR, "Error-distribution"); 
  ditheringFunc->setLabel(FLOYD, "Floyd-Steinberg"); 
  addPlug(_DITHER_IN =  new PlugIn<EnumType>(this, "Dither", ditheringFunc));

  EnumType *charFunc = new EnumType(N_CHARSET_TYPES, BASIC);
  charFunc->setLabel(BASIC, "7 Bit"); 
  charFunc->setLabel(EXTENDED, "8 Bit"); 
  addPlug(_CHAR_IN =  new PlugIn<EnumType>(this, "CharSet", charFunc));

  addPlug(_CONTRAST_IN = new PlugIn<ValueType>(this, "Contrast", new ValueType(0, 0, 255)));
  addPlug(_BRIGHTNESS_IN = new PlugIn<ValueType>(this, "Brightness", new ValueType(0, 0, 255)));

}

Gear_AsciiArt::~Gear_AsciiArt()
{
  aa_close(_aaContext);
}

void Gear_AsciiArt::init() 
{
  _aaHDParams = aa_defparams;
  _aaHDParams.width = 352 / 2;
  _aaHDParams.height = 240 / 2;
  _aaContext = aa_init(&mem_d, &_aaHDParams, NULL);
}

bool Gear_AsciiArt::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_AsciiArt::onUpdateSettings() 
{
}

void Gear_AsciiArt::runVideo()
{
  
  _inImage = _VIDEO_IN->type();
  ASSERT_ERROR(_inImage);

  if (_inImage->isNull())
    return;

  _sizeX = _inImage->width();
  _sizeY = _inImage->height();

  _outImage = _VIDEO_OUT->type();
  ASSERT_ERROR(_outImage);

  //memcpy(_outImage->data(), _inImage->data(), _inImage->size()*sizeof(RGBA));
 
  _inData = (unsigned char*)_inImage->data();
  
  for (int y=0; y<_sizeY; y++) 
  {
    for (int x=0; x<_sizeX; x++) 
    {
      aa_putpixel(_aaContext, x, y, *_inData++);
      _inData++;
      _inData++;
      _inData++;
    }
  }
  
  //aa_fastrender(_aaContext, 0, 0, _sizeX * 2, _sizeY * 2);
  
  //unsigned char *current = aa_text(_aaContext);

  //int width = _sizeX * ; //aa_imgwidth(_aaContext) / 2;
  //int height = _sizeY / 4; //aa_imgheight(_aaContext) / 2;
  
//   for ( int y=0; y<height; y++ ) 
//   {
//     for (int x=0; x<width; x++ ) 
//     {
//       printf("%c", current[y * width + x]);
//     }
//     printf("\n");
//   }

  //_outImage->resize(_sizeX, _sizeY);

}

