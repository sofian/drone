/* Gear_VideoMix.cpp
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

#include "Gear_VideoMix.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoMix, Gear_VideoMix, "VideoMix")

Gear_VideoMix::Gear_VideoMix(Engine *engine, std::string name) : Gear(engine, "VideoMix", name)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgO"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "ArgA", new ValueType(127, 0, 255)));

  EnumType *mixFunc = new EnumType(N_VIDEOMIX_TYPES, BLEND);
  mixFunc->setLabel(BLEND,"Blend");
  mixFunc->setLabel(SHADE,"Shade");
  mixFunc->setLabel(DARKEN,"Darken");
  mixFunc->setLabel(LIGHTEN,"Lighten");
  mixFunc->setLabel(HUE_ONLY,"Hue only");
  mixFunc->setLabel(SATURATION_ONLY,"Saturation only");
  mixFunc->setLabel(VALUE_ONLY,"Value only");
  mixFunc->setLabel(COLOR_ONLY,"Color only");
  mixFunc->setLabel(MULTIPLY,"Multiply");
  mixFunc->setLabel(DIVIDE,"Divide");
  mixFunc->setLabel(SCREEN,"Screen");
  mixFunc->setLabel(OVERLAY,"Overlay");
  mixFunc->setLabel(DODGE,"Dodge");
  mixFunc->setLabel(BURN,"Burn");
  mixFunc->setLabel(HARDLIGHT,"Hardlight");
  mixFunc->setLabel(SOFTLIGHT,"Softlight");
  mixFunc->setLabel(GRAIN_EXTRACT,"Grain extract");
  mixFunc->setLabel(GRAIN_MERGE,"Grain merge");
  mixFunc->setLabel(ADD,"Add");
  mixFunc->setLabel(SUBTRACT,"Subtract");
  mixFunc->setLabel(DIFFERENCE,"Difference");
  addPlug(_MIXFUNC_IN = new PlugIn<EnumType>(this, "MixFunc", mixFunc));
}

Gear_VideoMix::~Gear_VideoMix()
{

}

bool Gear_VideoMix::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoMix::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  _imageB = _VIDEO_IN_B->type();
  _outImage = _VIDEO_OUT->type();
  _mixType = CLAMP((eVideoMixType)_MIXFUNC_IN->type()->value(), BLEND, DIFFERENCE);

  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _imageOutSizeX = MAX(_imageASizeX, _imageBSizeX);
  _imageOutSizeY = MAX(_imageASizeY, _imageBSizeY);
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);

  _dataA = _imageA->data();
  _dataB = _imageB->data();
  _outData = _outImage->data();
  memset(_outData, 0, _outImage->size()*sizeof(RGBA));

  int amount = CLAMP((int)_AMOUNT_IN->type()->value(), 0, 255);
  switch (_mixType)
  {
  case BLEND:
    for (int y=0;y<_iterSizeY;y++)
    {
      blend_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   amount, _iterSizeX, SIZE_RGBA);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
    break;
  case SHADE:
    for (int y=0;y<_iterSizeY;y++)
    {
      shade_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   amount, _iterSizeX, SIZE_RGBA, 0);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
    break;
  case DARKEN:
    for (int y=0;y<_iterSizeY;y++)
    {
      darken_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                    _iterSizeX, SIZE_RGBA, SIZE_RGBA);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
    break;
  case LIGHTEN:
    for (int y=0;y<_iterSizeY;y++)
    {
      lighten_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                     _iterSizeX, SIZE_RGBA, SIZE_RGBA);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
    break;
  case HUE_ONLY:
    for (int y=0;y<_iterSizeY;y++)
    {
      hue_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      _iterSizeX, SIZE_RGBA, SIZE_RGBA);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
    break;
  case SATURATION_ONLY:
    for (int y=0;y<_iterSizeY;y++)
    {
      saturation_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                             _iterSizeX, SIZE_RGBA, SIZE_RGBA);
      _dataA += _imageASizeX;
      _dataB += _imageBSizeX;
      _outData += _imageOutSizeX;
    }
  break;
 case VALUE_ONLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     value_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                       _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case COLOR_ONLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     color_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                       _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case MULTIPLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     multiply_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                     _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case DIVIDE:
   for (int y=0;y<_iterSizeY;y++)
   {
     divide_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case SCREEN:
   for (int y=0;y<_iterSizeY;y++)
   {
     screen_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case OVERLAY:
   for (int y=0;y<_iterSizeY;y++)
   {
     overlay_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                    _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case DODGE:
   for (int y=0;y<_iterSizeY;y++)
   {
     dodge_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                  _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case BURN:
   for (int y=0;y<_iterSizeY;y++)
   {
     burn_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                 _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case HARDLIGHT:
   for (int y=0;y<_iterSizeY;y++)
   {
     hardlight_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case SOFTLIGHT:
   for (int y=0;y<_iterSizeY;y++)
   {
     softlight_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case GRAIN_EXTRACT:
   for (int y=0;y<_iterSizeY;y++)
   {
     grain_extract_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                          _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case GRAIN_MERGE:
   for (int y=0;y<_iterSizeY;y++)
   {
     grain_merge_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                        _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case ADD:
   for (int y=0;y<_iterSizeY;y++)
   {
     add_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case SUBTRACT:
   for (int y=0;y<_iterSizeY;y++)
   {
     subtract_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                     _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 case DIFFERENCE:
   for (int y=0;y<_iterSizeY;y++)
   {
     difference_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                       _iterSizeX, SIZE_RGBA, SIZE_RGBA);
     _dataA += _imageASizeX;
     _dataB += _imageBSizeX;
     _outData += _imageOutSizeX;
   }
   break;
 default:;
   memcpy(_outData, _dataA, _outImage->size() * sizeof(RGBA));
}

}
