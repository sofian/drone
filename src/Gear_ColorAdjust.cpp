#include "Gear_ColorAdjust.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_ColorAdjust, Gear_ColorAdjust, "ColorAdjust")

Gear_ColorAdjust::Gear_ColorAdjust(Engine *engine, std::string name) : Gear(engine, "ColorAdjust", name)
{
    _VIDEO_IN = addPlugVideoIn("ImgIN");
    _VIDEO_OUT = addPlugVideoOut("ImgOUT");
    _RED_IN = addPlugSignalIn("Red", 1.0f);
    _GREEN_IN = addPlugSignalIn("Green", 1.0f);
    _BLUE_IN = addPlugSignalIn("Blue", 1.0f);

}

Gear_ColorAdjust::~Gear_ColorAdjust()
{

}

bool Gear_ColorAdjust::ready()
{
    return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorAdjust::runVideo()
{
    _image = _VIDEO_IN->canvas();
    _outImage = _VIDEO_OUT->canvas();
    _outImage->allocate(_image->sizeX(), _image->sizeY());
    _data = _image->_data;    
    _outData = _outImage->_data;

    _iterSizeX = _image->sizeX();
    _iterSizeY = _image->sizeY();
          
//    register int mmxCols=_iterSizeX;
//    register int index;    

    //todo : fast float to int
    int red = (int)_RED_IN->buffer()[0];
    int green = (int)_GREEN_IN->buffer()[0];
    int blue = (int)_BLUE_IN->buffer()[0];
    
/*     _mmxColor=0;        */
/*     _mmxColor |= blue;  */
/*     _mmxColor <<= 16;   */
/*     _mmxColor |= green; */
/*     _mmxColor <<= 16;   */
/*     _mmxColor |= red;   */
    
    
    for (int y=0;y<_iterSizeY;y++)
    {    
        //_mmxImageIn = (int*)&_data[y*_iterSizeX];        
        //_mmxImageOut = (int*)&_outData[y*_iterSizeX];
        
        _imageIn = (unsigned char*)&_data[y*_iterSizeX];
        _imageOut = (unsigned char*)&_outData[y*_iterSizeX];
        for (int x=0;x<_iterSizeX;x++)
        {            
            _r = (*(_imageIn)*red) >> 8;
            _g = (*(_imageIn+1)*green) >> 8;
            _b = (*(_imageIn+2)*blue) >> 8;

            if (_r>255)
                *_imageOut = 255;
            else
                *_imageOut = _r;
            
            if (_g>255)
                *(_imageOut+1) = 255;
            else
                *(_imageOut+1) = _g;

            if (_b>255)
                *(_imageOut+2) = 255;
            else
                *(_imageOut+2) = _b;

           _imageIn+=4;
           _imageOut+=4;
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


    }
    __asm__("emms" : : );
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

