#include "Gear_VideoBlend.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoBlend, Gear_VideoBlend, "VideoBlend")

Gear_VideoBlend::Gear_VideoBlend(Engine *engine, std::string name) : Gear(engine, "VideoBlend", name)
{
    _VIDEO_IN_A = addPlugVideoIn("ImgA");
    _VIDEO_IN_B = addPlugVideoIn("ImgB");
    _VIDEO_OUT = addPlugVideoOut("ImgOUT");
    _ALPHA_IN = addPlugSignalIn("Alpha", 0.0f);//50%
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
    _imageA = _VIDEO_IN_A->canvas();
    _imageB = _VIDEO_IN_B->canvas();
    
    _outImage = _VIDEO_OUT->canvas();
    
    _imageOutSizeX = MAX(_imageA->sizeX(), _imageB->sizeX());
    _imageOutSizeY = MAX(_imageA->sizeY(), _imageB->sizeY());
    _outImage->allocate(_imageOutSizeX, _imageOutSizeY);
    
    //std::cout << _outImage->sizeX() << "x" << _outImage->sizeY() << std::endl;

    _dataA = _imageA->_data;    
    _dataB = _imageB->_data;    
    _outData = _outImage->_data;

    
    _imageASizeX = _imageA->sizeX();
    _imageASizeY = _imageA->sizeY();
    _imageBSizeX = _imageB->sizeX();
    _imageBSizeY = _imageB->sizeY();
    _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
    _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

    

    //todo fast float to int
    unsigned char tempAlpha = (unsigned char) _ALPHA_IN->buffer()[0];

    _mmxAlpha = tempAlpha;
    _mmxAlpha = _mmxAlpha << 32;
    _mmxAlpha |= tempAlpha;
          
    register int mmxCols=_iterSizeX/2;
    register int index;    
    
    for (int y=0;y<_iterSizeY;y++)
    {
        _mmxImageInA = (double*)&_dataA[y*_imageASizeX];
        _mmxImageInB = (double*)&_dataB[y*_imageBSizeX];
        _mmxImageOut = (double*)&_outData[y*_imageOutSizeX];

        for(index=0;index<mmxCols;index++)
        {
/*             __asm__ volatile (                                                                     */
/*                 "\n\t movq %1,%%mm0        \t# (u) load imageA"                                    */
/*                 "\n\t movq %2,%%mm1        \t# (u) load imageB"                                    */
/*                 "\n\t movq %3,%%mm2        \t# (u) load alpha"                                     */
/*                 "\n\t movq %%mm0,%%mm3        \t# (u) copy of ImageA"                              */
/*                 "\n\t movq %%mm1,%%mm4        \t# (u) copy of ImageB"                              */
/*                 "\n\t movq %%mm1,%%mm5        \t# (u) copy of ImageB again"                        */
/*                 "\n\t pxor %%mm7,%%mm7        \t# (u) mm7=0"                                       */
/*                 "\n\t punpcklbw %%mm7, %%mm0        \t# (u) mm1 -> low word ImageA"                */
/*                 "\n\t punpckhbw %%mm7, %%mm3        \t# (u) mm1 -> hi word ImageA"                 */
/*                 "\n\t punpcklbw %%mm7, %%mm1        \t# (u) mm1 -> low word ImageB"                */
/*                 "\n\t punpckhbw %%mm7, %%mm4        \t# (u) mm1 -> hi word ImageB"                 */
/*                 "\n\t psubw %%mm1, %%mm0        \t# (u) low ImageA - ImageB"                       */
/*                 "\n\t psubw %%mm4, %%mm3        \t# (u) hi ImageA - ImageB"                        */
/*                 "\n\t pmullw %%mm2, %%mm0        \t# (u) low * alpha"                              */
/*                 "\n\t pmullw %%mm2, %%mm3        \t# (u) hi * alpha"                               */
/*                 "\n\t psrlw $8, %%mm0        \t# (u) low /256"                                     */
/*                 "\n\t psrlw $8, %%mm3        \t# (u) hi /256"                                      */
/*                 "\n\t packuswb %%mm3, %%mm0        \t# (u) words -> pack to bytes with saturation" */
/*                 "\n\t paddb %%mm5, %%mm0        \t# (u) add src2"                                  */
/*                 "\n\t movq %%mm0, %0        \t# (u) copy to dest"                                  */
/*                 : "=m" (_mmxImageOut[index])  // this is %0, output                                */
/*                 : "m"  (_mmxImageInA[index]), // this is %1, image A                               */
/*                 "m"  (_mmxImageInB[index]), // this is 2, image B                                  */
/*                 "m"  (_mmxAlpha) // this is %3, Alpha                                              */
/*                 );                                                                                 */
        }

    }

    __asm__("emms" : : );
}

