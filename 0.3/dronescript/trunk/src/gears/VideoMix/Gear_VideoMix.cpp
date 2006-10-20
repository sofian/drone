/* Gear_VideoMix.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VideoMix(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoMix";
  gearInfo.classification = GearClassifications::video().composition().instance();
  return gearInfo;
}
}

Gear_VideoMix::Gear_VideoMix(Schema *schema, std::string uniqueName) : Gear(schema, "VideoMix", uniqueName)
{                                                                     
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA", false));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB", false));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut",true));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "ArgA", false, new ValueType(127, 0, 255)));

  EnumType *mixFunc = new EnumType(N_VIDEOMIX_TYPES, BLEND);
  mixFunc->setLabel(BLEND,"Blend");
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
  mixFunc->setLabel(SCALE,"Scale");
  addPlug(_MIXFUNC_IN = new PlugIn<EnumType>(this, "MixFunc", false, mixFunc));
}

Gear_VideoMix::~Gear_VideoMix()
{

}

void Gear_VideoMix::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  _imageB = _VIDEO_IN_B->type();
  _outImage = _VIDEO_OUT->type();

  _imageOutSizeX = MAX(_imageA->width(), _imageB->width());
  _imageOutSizeY = MAX(_imageA->height(), _imageB->height());
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);

  if (_imageA->isNull())
  {
    if (_imageB->isNull())
      return;
      
    memcpy(_outImage->data(), _imageB->data(), _imageA->size()*sizeof(RGBA));
    return;
  }
  else if (_imageB->isNull())
  {  
    memcpy(_outImage->data(), _imageA->data(), _imageA->size()*sizeof(RGBA));
    return;
  }


  _mixType = CLAMP((eVideoMixType)_MIXFUNC_IN->type()->value(), BLEND, SCALE);

  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _imageASizeX   *= SIZE_RGBA;
  _imageBSizeX   *= SIZE_RGBA;
  _imageOutSizeX *= SIZE_RGBA;

  GimpCompositeContext ctx;
  ctx.n_pixels=_iterSizeX;

  ctx.A = (guchar *)_imageA->data();
  ctx.B = (guchar *)_imageB->data();
  ctx.D = (guchar *)_outImage->data();
  
  int amount = CLAMP((int)_AMOUNT_IN->type()->value(), 0, 255);

  switch (_mixType)
  {
  case BLEND:
    ctx.blend.blend=amount;
    for (int y=0;y<_iterSizeY;y++)
    {
      gimp_composite_blend_any_any_any_generic(&ctx);
      ctx.A += _imageASizeX;
      ctx.B += _imageBSizeX;
      ctx.D += _imageOutSizeX;
    }
    break;
  case DARKEN:
    for (int y=0;y<_iterSizeY;y++)
    {
      gimp_composite_darken_any_any_any_generic(&ctx);
      ctx.A += _imageASizeX;
      ctx.B += _imageBSizeX;
      ctx.D += _imageOutSizeX;
    }
    break;
  case LIGHTEN:
    for (int y=0;y<_iterSizeY;y++)
    {
      gimp_composite_lighten_any_any_any_generic(&ctx);
      ctx.A += _imageASizeX;
      ctx.B += _imageBSizeX;
      ctx.D += _imageOutSizeX;
    }
    break;
  case HUE_ONLY:
    for (int y=0;y<_iterSizeY;y++)
    {
      gimp_composite_hue_any_any_any_generic(&ctx);
      ctx.A += _imageASizeX;
      ctx.B += _imageBSizeX;
      ctx.D += _imageOutSizeX;
    }
    break;
  case SATURATION_ONLY:
    gimp_composite_saturation_any_any_any_generic(&ctx);
    for (int y=0;y<_iterSizeY;y++)
    {
      ctx.A += _imageASizeX;
      ctx.B += _imageBSizeX;
      ctx.D += _imageOutSizeX;
    }
  break;
 case VALUE_ONLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_value_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case COLOR_ONLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_color_only_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case MULTIPLY:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_multiply_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case DIVIDE:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_divide_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case SCREEN:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_screen_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case OVERLAY:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_overlay_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case DODGE:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_dodge_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case BURN:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_burn_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case HARDLIGHT:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_hardlight_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case SOFTLIGHT:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_softlight_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case GRAIN_EXTRACT:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_grain_extract_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case GRAIN_MERGE:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_grain_merge_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case ADD:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_addition_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case SUBTRACT:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_subtract_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case DIFFERENCE:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_difference_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 case SCALE:
   for (int y=0;y<_iterSizeY;y++)
   {
     gimp_composite_scale_any_any_any_generic(&ctx);
     ctx.A += _imageASizeX;
     ctx.B += _imageBSizeX;
     ctx.D += _imageOutSizeX;
   }
   break;
 default:;
   memcpy(ctx.D, ctx.A, _outImage->size() * sizeof(RGBA));
}

}
