/* Gear_Edge.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#include "Gear_Edge.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_Edge, Gear_Edge, "Edge")

Gear_Edge::Gear_Edge(Engine *engine, std::string name) : Gear(engine, "Edge", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(127.0f, 0.0f, 255.0f)));
  NOTICE("For now, this edger is imperfect since it doesn't iterates through all points for efficiency reasons.");
}

Gear_Edge::~Gear_Edge()
{

}

bool Gear_Edge::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}



void Gear_Edge::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _data = (unsigned char*)_image->data();
  _outData = (unsigned char*)_outImage->data();

  unsigned char threshold = (unsigned char)CLAMP(_AMOUNT_IN->type()->value(), 0.0f, 255.0f);
  
  _sizeX = _image->width();
  _sizeY = _image->height();
  
  for (int y=1;y<_sizeY-2;y++)
  {
    // Set row iterators.
    _iterUp =      (unsigned char*) (_image->row(y-1) +  1);
    _iterCenter =  (unsigned char*) (_image->row(y)   +  1);
    _iterDown =    (unsigned char*) (_image->row(y+1) +  1);
    _iterLeft =    (unsigned char*) (_image->row(y)   +  0);
    _iterRight =   (unsigned char*) (_image->row(y)   +  2);
    
    _iterOutData = (unsigned char*) (_outImage->row(y) + 1);
    
    for (int x=1;x<_sizeX-2;x++)
    {
      for (int z=0; z<SIZE_RGBA; ++z)
      {
        // Compute the kernel function.
        *_iterOutData =
          (*_iterCenter++ << 2)
          - *_iterUp++
          - *_iterDown++
          - *_iterLeft++
          - *_iterRight++;

        // Check threshold.
        if (*_iterOutData <= threshold)
          *_iterOutData = 0;

        // Update iterators.
        _iterOutData++;
      }
      
      /*
      
      
      _mmxImageInL0 = (double*)&_data[(y-1)*_iterSizeX];
      _mmxImageInL02 = (double*)&_data[((y-1)*_iterSizeX)+2];
      _mmxImageInL1 = (double*)&_data[(y*_iterSizeX)];
      _mmxImageInL12 = (double*)&_data[(y*_iterSizeX)+2];
      _mmxImageInL2 = (double*)&_data[(y+1)*_iterSizeX];
      _mmxImageInL22 = (double*)&_data[((y+1)*_iterSizeX)+2];

      _mmxImageOut = (double*)&_outData[(y*_iterSizeX)+1];
      */

    }

  }
//    __asm__("emms" : :);
}


/*             __asm__ volatile (                                        */
/*                 "\n\t movq %1,%%mm0        \t# (u) load imageInL0"    */
/*                 "\n\t movq %2,%%mm1        \t# (u) load imageInL02"   */
/*                 "\n\t movq %3,%%mm2        \t# (u) load imageInL2"    */
/*                 "\n\t movq %4,%%mm3        \t# (u) load imageInL22"   */
/*                 "\n\t psubusb %%mm0,%%mm2   \t# (u) L2 = L0 - L2"     */
/*                 "\n\t psubusb %%mm1,%%mm3   \t# (u) L2 = L0 - L2"     */
/*                 "\n\t paddusb %%mm2,%%mm3   \t# (u) L2 = L0 - L2"     */
/*                 "\n\t movq %%mm3,%0        \t# (u) store result "     */
/*                 : "=m" (_mmxImageOut[index])  // this is %0, output   */
/*                 : "m"  (_mmxImageInL0[index]), // this is %1, image A */
/*                 "m"  (_mmxImageInL02[index]), // this is %1, image A  */
/*                 "m"  (_mmxImageInL2[index]), // this is %1, image A   */
/*                 "m"  (_mmxImageInL22[index]) // this is %1, image A   */
/*                 );                                                    */


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

