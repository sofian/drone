#include "Gear_Contrast.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"



Register_Gear(MAKERGear_Contrast, Gear_Contrast, "Contrast")

Gear_Contrast::Gear_Contrast(Engine *engine, std::string name) : Gear(engine, "Contrast", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_CONTRAST_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(1.0f)));

}

Gear_Contrast::~Gear_Contrast()
{

}

bool Gear_Contrast::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Contrast::runVideo()
{
  _image = _VIDEO_IN->type()->image();
  _outImage = _VIDEO_OUT->type()->image();
  _outImage->resize(_image->width(), _image->height());
  _data = _image->data();    
  _outData = _outImage->data();

  _iterSizeX = _image->width();
  _iterSizeY = _image->height();

  int contrast = (int)_CONTRAST_IN->type()->value();

  for (int y=0;y<_iterSizeY;y++)
  {
    _imageIn = (unsigned char*)&_data[y*_iterSizeX];
    _imageOut = (unsigned char*)&_outData[y*_iterSizeX];

    for (int x=0;x<_iterSizeX;x++)
    {
      _r = 128 + ((((contrast) * ( *(_imageIn) - 128)))>>8);
      _g = 128 + ((((contrast) * ( *(_imageIn+1) - 128)))>>8);
      _b = 128 + ((((contrast) * ( *(_imageIn+2) - 128)))>>8);

      if (_r>255)
        *_imageOut = 255;
      else
        if (_r<0)
        *_imageOut = 0;
      else
        *_imageOut = _r;

      if (_g>255)
        *(_imageOut+1) = 255;
      else
        if (_g<0)
        *(_imageOut+1) = 0;
      else
        *(_imageOut+1) = _g;

      if (_b>255)
        *(_imageOut+2) = 255;
      else
        if (_b<0)
        *(_imageOut+2) = 0;
      else
        *(_imageOut+2) = _b;

      _imageIn+=4;
      _imageOut+=4;
    }
  }
}


/*         for(index=0;index<mmxCols;index++)                                                                        */
/*         {                                                                                                         */
/*             __asm__ volatile (                                                                                    */
/*                 "\n\t movq %1,%%mm0                \t# (u) load imageIn"                                          */
/*                 "\n\t movq %2,%%mm1                \t# (u) load median"                                           */
/*                 "\n\t movq %3,%%mm2                \t# (u) load contrast"                                         */
/*                 //"\n\t psubb %%mm1,%%mm0            \t# (u) imageIn + median"                                    */
/*                 "\n\t movq %%mm0,%%mm4             \t# (u) copy of image"                                         */
/*                 "\n\t pxor %%mm3,%%mm3             \t# (u) zero mm3"                                              */
/*                 "\n\t punpcklbw %%mm3,%%mm0        \t# (u) low 32bits 4Bytes->4Words"                             */
/*                 "\n\t punpckhbw %%mm3,%%mm4        \t# (u) high 32bits 4Bytes->4Words"                            */
/*                 "\n\t psubw %%mm1,%%mm0            \t# (u) imageIn - median"                                      */
/*                 "\n\t psubw %%mm1,%%mm4            \t# (u) imageIn - median"                                      */
/*                 //"\n\t movq %%mm0,%%mm5             \t# (u) copy of image"                                       */
/*                 //"\n\t movq %%mm4,%%mm6             \t# (u) copy of image"                                       */
/*                 //"\n\t pmulhuw %%mm2, %%mm0          \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0" */
/*                 "\n\t psrlw $8, %%mm0              \t# (u) %%mm0 >> 8 (/256)"                                     */
/*                 "\n\t psrlw $8, %%mm4              \t# (u) %%mm4 >> 8 (/256)"                                     */
/*                 "\n\t pmullw %%mm2, %%mm0          \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"    */
/*                 "\n\t pmullw %%mm2, %%mm4          \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"    */
/*                 "\n\t paddw %%mm1,%%mm0            \t# (u) imageIn + median"                                      */
/*                 "\n\t paddw %%mm1,%%mm4            \t# (u) imageIn + median"                                      */
/*                 "\n\t packuswb %%mm4,%%mm0         \t# (u) 4words 4Words->(hi)4bytes (low)4Bytes"                 */
/*                 //"\n\t paddusb %%mm1,%%mm0            \t# (u) imageIn + median"                                  */
/*                 //"\n\t paddusb %%mm1,%%mm4            \t# (u) imageIn + median"                                  */
/*                 //"\n\t paddusb %%mm1,%%mm0          \t# (u) imageIn + median"                                    */
/*                 //"\n\t packuswb %%mm0,%%mm0            \t# (u) 4Words->4Bytes"                                   */
/*                 //"\n\t punpcklbw %%mm2,%%mm3        \t# (u) low 32bits 4Bytes->4Words"                           */
/*                 //"\n\t pmaddwd %%mm1, %%mm0         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"  */
/*                 //"\n\t pmullw %%mm1, %%mm0         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"   */
/*                 //"\n\t pmulhw %%mm1, %%mm3         \t# (u) %%mm0(high and low) = 4Words %%mm1 *  4Words %%mm0"   */
/*                 //"\n\t psllw $16, %%mm3               \t# (u) %%mm0 >> 8 (/256)"                                 */
/*                 //"\n\t por %%mm3, %%mm0               \t# (u) %%mm0 >> 8 (/256)"                                 */
/*                 //"\n\t psrlw $8, %%mm0               \t# (u) %%mm0 >> 8 (/256)"                                  */
/*                 //"\n\t packuswb %%mm0,%%mm0            \t# (u) 4Words->4Bytes"                                   */
/*                 "\n\t movq %%mm0,%0        \t# (u) store result "                                                 */
/*                 : "=m" (_mmxImageOut[index])  // this is %0, output                                               */
/*                 : "m"  (_mmxImageIn[index]), // this is %1, image A                                               */
/*                  "m"  (medianWordAligned), // this is %1, image A                                                 */
/*                  "m"  (contrastWordAligned) // this is %1, image A                                                */
/*                 );                                                                                                */
/*         }                                                                                                         */

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

