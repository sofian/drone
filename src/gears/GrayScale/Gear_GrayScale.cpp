/* Gear_GrayScale.cpp
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

#include "Gear_GrayScale.h"
#include "Engine.h"
#include "Utils.h"

#include "Array2DType.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_GrayScale(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "GrayScale";
  gearInfo.classification = GearClassifications::video().color().instance();
  return gearInfo;
}
}

Gear_GrayScale::Gear_GrayScale(Schema *schema, std::string uniqueName) : Gear(schema, "GrayScale", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this,"ImgOUT"));
  addPlug(_CHANNEL_OUT = new PlugOut<VideoChannelType>(this,"ChOUT"));
}

Gear_GrayScale::~Gear_GrayScale()
{

}

bool Gear_GrayScale::ready()
{
  return(_VIDEO_IN->connected() && (_VIDEO_OUT->connected() || _CHANNEL_OUT->connected()));
}

void Gear_GrayScale::runVideo()
{
  _image = _VIDEO_IN->type();
  
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _outChannel->resize(_image->width(), _image->height());

  //! XXX si les deux sont connectés, y a moyen de faire ça plus vite...
  
  if (_CHANNEL_OUT->connected())
  {
    if (_image->isGray()) // already gray
      extractChannel((unsigned char*)_outChannel->data(), _image->data(), _image->size(), IDX_RGBA_R); // extract one of the channels
    else
      grayscaleChannel((unsigned char*)_outChannel->data(), _image->data(), _image->size());
  }

  if (_VIDEO_OUT->connected())
  {
    if (_image->isGray()) // already gray
      memcpy(_outImage->data(), _image->data(), _image->size() * sizeof(RGBA));
    else
      grayscaleRGBA(_outImage->data(), _image->data(), _image->size());

    // XXX si c'est trop slow, utiliser fastrgb2gray

    // Output image is now gray.
    _outImage->setIsGray(true);
  }
  
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

