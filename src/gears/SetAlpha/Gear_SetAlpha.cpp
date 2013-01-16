/* Gear_GrayScale.cpp
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

#include "Gear_SetAlpha.h"
#include "Engine.h"

#include "AlphaComposite.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear()
{
  return new Gear_SetAlpha();
}


}

Gear_SetAlpha::Gear_SetAlpha() : Gear("SetAlpha")
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_ALPHA_MASK_IN = new PlugIn<VideoChannelType>(this, "Mask", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this,"ImgOUT", true));
}

Gear_SetAlpha::~Gear_SetAlpha()
{

}

void Gear_SetAlpha::runVideo()
{
  _image = _VIDEO_IN->type();
  _mask = _ALPHA_MASK_IN->type();
  
  if (_image->isNull() || _mask->isNull())
    return;

  ASSERT_ERROR(_image->width() == _mask->width() && _image->height() == _mask->height());

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  std::copy(_image->begin(), _image->end(), _outImage->begin());
  
  alpha_set((unsigned char*)_outImage->data(), _mask->data(), _outImage->size());
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

