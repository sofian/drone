/* Gear_GrayScale.cpp
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

#include "Gear_GrayScale.h"
#include "Engine.h"

#include "Array2DType.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_GrayScale, Gear_GrayScale, "GrayScale")

Gear_GrayScale::Gear_GrayScale(Engine *engine, std::string name) : Gear(engine, "GrayScale", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this,"ImgOUT"));
}

Gear_GrayScale::~Gear_GrayScale()
{

}

bool Gear_GrayScale::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_GrayScale::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _size = _image->size();

  if (_image->isGray()) // already gray
    memcpy(_outImage->data(), _image->data(), _image->size() * sizeof(RGBA));
  else
  {
    _imageIn  = (unsigned char*)_image->data();
    _imageOut = (unsigned int*) _outImage->data();

    int total;
    
    for (int p=0; p<_size; ++p)
    {
      // add everything
      // 0.25 * R + 0.5 * G + 0.25 * B
      // XXX another option (a little less efficient) comes from Qt : they use (r*11 + g*16 + b*5)/32 (division could be obtained with a shift)
      total = *_imageIn++;
      total += *_imageIn;
      total += *_imageIn++;
      total += *_imageIn++;
      _imageIn++;
      
      // divide by 4
      total >>= 2;
      
      //        R = total | G = total | B = total
      *_imageOut++ = total | total<<8 | total <<16;
    }
  }

  // Output image is now gray.
  _outImage->setIsGray(true);
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

