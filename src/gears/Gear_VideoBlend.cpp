/* Gear_VideoBlend.cpp
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

#include "Gear_VideoBlend.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoBlend, Gear_VideoBlend, "VideoBlend")

Gear_VideoBlend::Gear_VideoBlend(Engine *engine, std::string name) : Gear(engine, "VideoBlend", name)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoTypeRGBA>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoTypeRGBA>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_ALPHA_IN = new PlugIn<ValueType>(this, "Alpha", new ValueType(0.0f)));//50%
}

Gear_VideoBlend::~Gear_VideoBlend()
{

}

bool Gear_VideoBlend::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}



void Gear_VideoBlend::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  _imageB = _VIDEO_IN_B->type();

  _outImage = _VIDEO_OUT->type();

  _imageOutSizeX = MAX(_imageA->width(), _imageB->width());
  _imageOutSizeY = MAX(_imageA->height(), _imageB->height());
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);

  //std::cout << _outImage->width() << "x" << _outImage->height() << std::endl;

  _dataA = _imageA->data();    
  _dataB = _imageB->data();    
  _outData = _outImage->data();


  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);



  //todo fast float to int
  _mmxAlpha = (unsigned char) _ALPHA_IN->type()->value();
  unsigned char tempAlpha[8];
  tempAlpha[0] = _mmxAlpha;
  tempAlpha[1] = 0;
  tempAlpha[2] = _mmxAlpha;
  tempAlpha[3] = 0;
  tempAlpha[4] = _mmxAlpha;
  tempAlpha[5] = 0;
  tempAlpha[6] = _mmxAlpha;
  tempAlpha[7] = 0;


  register int mmxCols=_iterSizeX/2;
  register int index;    

  for (int y=0;y<_iterSizeY;y++)
  {
    _mmxImageInA = (double*)&_dataA[y*_imageASizeX];
    _mmxImageInB = (double*)&_dataB[y*_imageBSizeX];
    _mmxImageOut = (double*)&_outData[y*_imageOutSizeX];

    for (index=0;index<mmxCols;index++)
    {
      __asm__ volatile (
                       "\n\t movq %1,%%mm0        \t# (u) load imageA"
                       "\n\t movq %2,%%mm1        \t# (u) load imageB"
                       "\n\t movq %%mm0,%%mm2        \t# (u) load imageB"
                       "\n\t movq %%mm1,%%mm3        \t# (u) load imageB"
                       "\n\t movq %%mm1,%%mm4        \t# (u) load imageB"
                       "\n\t movq %3,%%mm6        \t# (u) load alpha"                
                       "\n\t pxor %%mm7,%%mm7        \t# (u) zero mm7"
                       "\n\t punpcklbw %%mm7, %%mm0    "                
                       "\n\t punpcklbw %%mm7, %%mm1    "                
                       "\n\t punpckhbw %%mm7, %%mm2    "                
                       "\n\t punpckhbw %%mm7, %%mm3    "                
                       "\n\t psubw %%mm1, %%mm0    "
                       "\n\t psubw %%mm3, %%mm2    "
                       "\n\t pmullw %%mm6, %%mm0    "                
                       "\n\t pmullw %%mm6, %%mm2    "                
                       "\n\t psrlw $8, %%mm0    "                                
                       "\n\t psrlw $8, %%mm2    "                                                
                       "\n\t packuswb %%mm2, %%mm0    "                          
                       "\n\t paddb %%mm4, %%mm0    "                
                       "\n\t movq %%mm0, %0    "

                       : "=m" (_mmxImageOut[index])  // this is %0, output
                       : "m"  (_mmxImageInA[index]), // this is %1, image A
                       "m"  (_mmxImageInB[index]), // this is 2, image B
                       "m"  (tempAlpha[0]) // this is %3, Alpha
                       );
    }

  }

  __asm__("emms" : : );
}

