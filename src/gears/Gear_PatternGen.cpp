#include "Gear_PatternGen.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_PatternGen, Gear_PatternGen, "PatternGen")

const std::string Gear_PatternGen::SETTING_SIZE_X = "SizeX";
const std::string Gear_PatternGen::SETTING_SIZE_Y = "SizeY";

/* unsigned long xcng(void){  // takes 60 nanosecs, passes all tests */
/*     static unsigned long x=123456789,y=521288629,z,w;             */
/*     z=x+y; x=y;  w=(z<<8); y=w+17*(z>>24);                        */
/*     if (z<x) y=y+4352;                                            */
/*     if (y<w) y=y+17;                                              */
/*     return( y );}                                                 */

Gear_PatternGen::Gear_PatternGen(Engine *engine, std::string name) : Gear(engine, "PatternGen", name)
{    
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _PARAM_IN = addPlugSignalIn("Parm", 1.0f);

  _settings.add(Property::INT, SETTING_SIZE_X)->valueInt(256);    
  _settings.add(Property::INT, SETTING_SIZE_Y)->valueInt(256);    

  updateSettings();
}

Gear_PatternGen::~Gear_PatternGen()
{

}

void Gear_PatternGen::onUpdateSettings()
{    
  _VIDEO_OUT->canvas()->allocate(_settings.get(SETTING_SIZE_X)->valueInt(), _settings.get(SETTING_SIZE_Y)->valueInt());
}

bool Gear_PatternGen::ready()
{
  return(_VIDEO_OUT->connected());
}

void Gear_PatternGen::runVideo()
{
  _outImage = _VIDEO_OUT->canvas();    
  _outData = _outImage->_data;

  _iterSizeX = _outImage->sizeX();
  _iterSizeY = _outImage->sizeY();

  //int PatternGen = (int)((float)((_PatternGen_IN->buffer()[0]+1.0f) * 1024.0f));    

  for (int y=0;y<_iterSizeY;y++)
  {
    _imageOut = (unsigned char*)&_outData[(y*_iterSizeX)];        
    for (int x=0;x<_iterSizeX;x++)
    {
/*             *(_imageOut) = x*y*(_PARAM_IN->buffer()[0]);                */
/*             *(_imageOut+1) = x*y*(_PARAM_IN->buffer()[0]);//rand()%256; */
/*             *(_imageOut+2) = x*y*(_PARAM_IN->buffer()[0]);//rand()%256; */
/*             _imageOut+=4;                                               */
    }
  }
}


/*         for(index=0;index<mmxCols;index++)                                                                        */
/*         {                                                                                                         */
/*             __asm__ volatile (                                                                                    */
/*                 "\n\t movq %1,%%mm0                \t# (u) load imageIn"                                          */
/*                 "\n\t movq %2,%%mm1                \t# (u) load median"                                           */
/*                 "\n\t movq %3,%%mm2                \t# (u) load PatternGen"                                         */
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
/*                  "m"  (PatternGenWordAligned) // this is %1, image A                                                */
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

