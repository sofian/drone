/* Gear_AlphaMix.cpp
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

#include "Gear_AlphaMix.h"                       
#include "Engine.h"

#include <iostream>




extern "C" {
Gear* makeGear()
{
  return new Gear_AlphaMix();
}

}

Gear_AlphaMix::Gear_AlphaMix() : Gear("AlphaMix")
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA", true));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", true));

  EnumType *mixFunc = new EnumType((int)N_ALPHAMIX_TYPES, (int)OVER);
  mixFunc->setLabel((int)OVER,"Over");
  mixFunc->setLabel((int)IN,"In");
  mixFunc->setLabel((int)OUT,"Out");
  mixFunc->setLabel((int)ATOP,"Atop");
  mixFunc->setLabel((int)XOR,"Xor");
  addPlug(_MIXFUNC_IN = new PlugIn<EnumType>(this, "MixFunc", false, mixFunc));

  _imageA = new VideoRGBAType();
  _imageB = new VideoRGBAType();
}

Gear_AlphaMix::~Gear_AlphaMix()
{
  delete _imageA;
  delete _imageB;
}

void Gear_AlphaMix::runVideo()
{
//   _imageA = _VIDEO_IN_A->type();
//   _imageB = _VIDEO_IN_B->type();

//   _outImage = _VIDEO_OUT->type();
//   _outImage->resize(_imageA->width(), _imageA->height());

//   for (int i=0; i<(int)_outImage->size(); ++i)
//   {
//     const RGBA& inA = _imageA->operator[](i);
//     const RGBA& inB = _imageB->operator[](i);
//     RGBA& out = _outImage->operator[](i);

//     // premultiply
//     float 
//     out.r = inA + inB * (255 - inA.a);
    
//     _outImage.r = _imageA[i] (255 - _imageA[i].a) 
//     w2 = (255 - (float)src1[IDX_RGBA_A]);
//     for (b=0; b<SIZE_RGB; ++b)
//       dst[b] = CLAMP0255( (int) (src1[b] + ((int)src2[b] * w2) / 255) ); // XXX le clamping est pas necessaire, normalement...
//     dst[IDX_RGBA_A] = CLAMP0255( (int) (src1[IDX_RGBA_A] + ((int)src2[IDX_RGBA_A] * w2) / 255) );
//     src1+=SIZE_RGBA;
//     src2+=SIZE_RGBA;
//     dst+=SIZE_RGBA;
//   }
  _imageA->setIsAlphaPremultiplied(false);
  _imageB->setIsAlphaPremultiplied(false);
  _imageA->resize(_VIDEO_IN_A->type()->width(), _VIDEO_IN_A->type()->height());
  std::copy(_VIDEO_IN_A->type()->begin(), _VIDEO_IN_A->type()->end(), _imageA->begin());
  _imageB->resize(_VIDEO_IN_B->type()->width(), _VIDEO_IN_B->type()->height());
  std::copy(_VIDEO_IN_B->type()->begin(), _VIDEO_IN_B->type()->end(), _imageB->begin());
  //std::cout << (int)_imageA->operator[](0).r << " " << (int)_imageA->operator[](0).a << " ";

  _imageA->premultiplyAlpha();
  _imageB->premultiplyAlpha();

  //  std::cout << (int)_imageA->operator[](0).r << " " << (int)_imageA->operator[](0).a << std::endl;
    
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

  _mixType = CLAMP((eAlphaMixType)_MIXFUNC_IN->type()->value(), OVER, XOR);

  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _imageASizeX *= SIZE_RGBA;
  _imageBSizeX *= SIZE_RGBA;
  _imageOutSizeX *= SIZE_RGBA;

  _imageInA = (const unsigned char *)_imageA->data();
  _imageInB = (const unsigned char *)_imageB->data();
  _imageOut = (unsigned char *)_outImage->data();

  switch (_mixType)
  {
  case OVER:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_over(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case IN:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_in(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case OUT:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_out(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case ATOP:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_atop(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
  case XOR:
    for (int y=0;y<_iterSizeY;y++)
    {
      alpha_xor(_imageOut, _imageInA, _imageInB, _iterSizeX);
      _imageInA += _imageASizeX;
      _imageInB += _imageBSizeX;
      _imageOut += _imageOutSizeX;
    }
    break;
 default:;
   memcpy(_imageOut, _imageInA, _outImage->size() * sizeof(RGBA));
  }

  _outImage->demultiplyAlpha(); // XXX on devrait pas avoir à faire ça...
}

