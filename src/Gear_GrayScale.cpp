#include "Gear_GrayScale.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_GrayScale, Gear_GrayScale, "GrayScale")

Gear_GrayScale::Gear_GrayScale(Engine *engine, std::string name) : Gear(engine, "GrayScale", name)
{
    _VIDEO_IN = addPlugVideoIn("ImgIN");
    _VIDEO_OUT = addPlugVideoOut("ImgOUT");

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
    _image = _VIDEO_IN->canvas();
    _outImage = _VIDEO_OUT->canvas();
    _outImage->allocate(_image->sizeX(), _image->sizeY());
    _data = _image->_data;    
    _outData = _outImage->_data;
    
    _size = _image->sizeX() * _image->sizeY();

    _imageIn  = (unsigned char*)_data;
    _imageOut = (unsigned int*)_outData;
    
    int total;
    
    for (int p=0; p<_size; ++p)
    {
      // add everything
      total = 255;
      total += *_imageIn++;
      total += *_imageIn++;
      total += *_imageIn++;
      _imageIn++;
      
      // divide by 4
      total >>= 2;
      
      //        R = total | G = total | B = total
      *_imageOut++ = total | total<<8 | total <<16;
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

