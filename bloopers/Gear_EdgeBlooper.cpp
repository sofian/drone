#include "Gear_EdgeBlooper.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_EdgeBlooper, Gear_EdgeBlooper, "EdgeBlooper")

Gear_EdgeBlooper::Gear_EdgeBlooper(Engine *engine, std::string name) : Gear(engine, "EdgeBlooper", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(127.0f, 0.0f, 255.0f)));
}

Gear_EdgeBlooper::~Gear_EdgeBlooper()
{

}

bool Gear_EdgeBlooper::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}



void Gear_EdgeBlooper::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _data = (unsigned char*)_image->data();
  _outData = (unsigned char*)_outImage->data();

  unsigned char threshold = (unsigned char)CLAMP(_AMOUNT_IN->type()->value(), 0.0f, 255.0f);

  unsigned char *it;
  
  _sizeX = _image->width();
  _sizeY = _image->height();

  NOTICE("For now, this edge is imperfect since it doesn't iterates through all points for efficiency reasons.");
  
  for (int y=1;y<_sizeY-2;y++)
  {
    // Set row iterators.
    _iterPrevRow = _image->row(y-1)  + 1;
    _iterCurrRow = _image->row(y)    + 1;
    _iterNextRow = _image->row(y+1)  + 1;

    _iterOutData = _outImage->row(y) + 1;
    
    for (int x=1;x<_sizeX-2;x++)
    {
      // Compute the kernel function.
      *_iterOutData = *_iterCurrRow;
      *_iterOutData << 2;
      *_iterOutData -= *_iterPrevRow;
      *_iterOutData -= *_iterNextRow;
      *_iterOutData -= *(_iterCurrRow-1);
      *_iterOutData -= *(_iterCurrRow+1);

      // Check threshold.
      it = (unsigned char*) _iterOutData;
      for (int z=0; z<SIZE_RGBA; ++z)
      {
        if (*it <= threshold)
          *it = 0;
        it++;
      }

      // Update iterators.
      _iterOutData++;
      _iterPrevRow++;
      _iterCurrRow++;
      _iterNextRow++;
 
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

