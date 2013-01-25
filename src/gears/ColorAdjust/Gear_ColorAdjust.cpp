/* Gear_ColorAdjust.cpp
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

#include "Gear_ColorAdjust.h"
#include "Engine.h"

#include <iostream>




extern "C" {
Gear* makeGear()
{
  return new Gear_ColorAdjust();
}

}

Gear_ColorAdjust::Gear_ColorAdjust() : Gear("ColorAdjust")
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_RED_IN = new PlugIn<ValueType>(this, "Red", false, new ValueType(1.0f)));
  addPlug(_GREEN_IN = new PlugIn<ValueType>(this, "Green", false, new ValueType(1.0f)));
  addPlug(_BLUE_IN = new PlugIn<ValueType>(this, "Blue", false, new ValueType(1.0f)));
}

Gear_ColorAdjust::~Gear_ColorAdjust()
{

}

void Gear_ColorAdjust::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _size = _image->size();
  
  //todo : fast float to int
  float red   = _RED_IN->type()->value();
  float green = _GREEN_IN->type()->value();
  float blue  = _BLUE_IN->type()->value();

/*     _mmxColor=0;        */
/*     _mmxColor |= blue;  */
/*     _mmxColor <<= 16;   */
/*     _mmxColor |= green; */
/*     _mmxColor <<= 16;   */
/*     _mmxColor |= red;   */

  _imageIn = (unsigned char*)_image->data();
  _imageOut = (unsigned char*)_outImage->data();
  for (int p=0; p<_size; ++p)
  {
    // Set temporary values.
    _r = (int)(*(_imageIn++) * red);
    _g = (int)(*(_imageIn++) * green);
    _b = (int)(*(_imageIn++) * blue);

    // Clamp and copy to output.
    *_imageOut++ = MIN(_r, (short)255);
    *_imageOut++ = MIN(_g, (short)255);
    *_imageOut++ = MIN(_b, (short)255);

    // Skip alpha.
    _imageIn++;
    _imageOut++;
  }

    //todo : finilize mmx version

/*         for(index=0;index<mmxCols;index++)                                                                       */
/*         {                                                                                                        */
/*             __asm__ volatile (                                                                                   */
/*                 "\n\t movd %1,%%mm0                \t# (u) load imageIn"                                         */
/*                 "\n\t movd %1,%%mm3                \t# (u) load imageIn"                                         */
/*                 "\n\t movq %2,%%mm1                \t# (u) load color"                                           */
/*                 "\n\t pxor %%mm2,%%mm2                \t# (u) load color"                                        */
/*                 "\n\t punpcklbw %%mm2,%%mm0        \t# (u) low 32bits 4Bytes->4Words"                            */
/*                 "\n\t punpcklbw %%mm2,%%mm3        \t# (u) low 32bits 4Bytes->4Words"                            */
/*                 //"\n\t pmaddwd %%mm1, %%mm0         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0" */
/*                 "\n\t pmullw %%mm1, %%mm0         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"    */
/*                 "\n\t pmulhw %%mm1, %%mm3         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"    */
/*                 "\n\t psllw $16, %%mm3               \t# (u) %%mm0 >> 8 (/256)"                                  */
/*                 "\n\t por %%mm3, %%mm0               \t# (u) %%mm0 >> 8 (/256)"                                  */
/*                 "\n\t psrlw $8, %%mm0               \t# (u) %%mm0 >> 8 (/256)"                                   */
/*                 "\n\t packuswb %%mm0,%%mm0            \t# (u) 4Words->4Bytes"                                    */
/*                 "\n\t movd %%mm0,%0        \t# (u) store result "                                                */
/*                 : "=m" (_mmxImageOut[index])  // this is %0, output                                              */
/*                 : "m"  (_mmxImageIn[index]), // this is %1, image A                                              */
/*                  "m"  (_mmxColor) // this is %1, image A                                                         */
/*                 );                                                                                               */
/*         }                                                                                                        */


//   }
//   __asm__("emms" : : );
}



/*             for (int yw=_startY,yk=0;yw<=_endY;yw++,yk++)          */
/*                 for (int xw=_startX,xk=0;xw<=_endX;xw++,xk++)      */
/*                 {                                                  */
/*                     _tempData = _data + (((yw*_sizeX) + xw) << 2); */
/*                                                                    */
/*                     _accR+=*(_tempData) * _kernel[(yk*3)+xk];      */
/*                     _accG+=*(_tempData+1) * _kernel[(yk*3)+xk];    */
/*                     _accB+=*(_tempData+2) * _kernel[(yk*3)+xk];    */
/*                 }                                                  */
/*                                                                    */
/*                                                                    */
/*             _tempData = _data + (((y*_sizeX) + x) << 2);           */
/*             _finalR = (_accR + *_tempData);                        */
/*             _finalG = (_accR + *(_tempData+1));                    */
/*             _finalB = (_accR + *(_tempData+2));                    */
/*                                                                    */
/*             if (_finalR > 255)                                     */
/*                 _finalR=255;                                       */
/*             if (_finalG > 255)                                     */
/*                 _finalG=255;                                       */
/*             if (_finalB > 255)                                     */
/*                 _finalB=255;                                       */
/*                                                                    */
/*             if (_finalR<0)                                         */
/*                 _finalR=0;                                         */
/*             if (_finalG<0)                                         */
/*                 _finalG=0;                                         */
/*             if (_finalB<0)                                         */
/*                 _finalB=0;                                         */
/*                                                                    */
/*             _outData[(y*_sizeX) + x].R = _finalR;                  */
/*             _outData[(y*_sizeX) + x].G = _finalG;                  */
/*             _outData[(y*_sizeX) + x].B = _finalB;                  */
/*                                                                    */
/*             _accR=0;                                               */
/*             _accG=0;                                               */
/*             _accB=0;                                               */
/*                                                                    */


