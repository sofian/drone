#include "Gear_HalfToning.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"



Register_Gear(MAKERGear_HalfToning, Gear_HalfToning, "HalfToning")

Gear_HalfToning::Gear_HalfToning(Engine *engine, std::string name)
  : Gear(engine, "HalfToning", name), _carryLine0(0), _carryLine1(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
}

Gear_HalfToning::~Gear_HalfToning()
{
  free(_carryLine0);
  free(_carryLine1);
}

bool Gear_HalfToning::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_HalfToning::runVideo()
{
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  
  _data = _image->_data;    
  _outData = _outImage->_data;

  /* allocate _carryLine0 and _carryLine1 */
  if (_carryLine0)
    _carryLine0-=1;
  if (_carryLine1)
    _carryLine1-=1;
  
  _carryLine0 = (double*)realloc(_carryLine0, (_sizeX+2)*sizeof(double));
  _carryLine1 = (double*)realloc(_carryLine1, (_sizeX+2)*sizeof(double));
  memset(_carryLine0, 0, (_sizeX+2)*sizeof(double));
  memset(_carryLine1, 0, (_sizeX+2)*sizeof(double));
  _carryLine0+=1;
  _carryLine1+=1;  
 
  unsigned char *iterData;
  unsigned char *iterOutData;

  int xstart, xstop, xstep, dir, iterDataStep;
  int input, intensity;
  double threshold = 127.5, corrected_level, diff;

  for (int y = 0; y < _sizeY; ++y)
  {
    if (TEST_ODD(y))
    {
      dir = TO_LEFT;
      xstart = _sizeX-1; xstop = -1; xstep = -1; iterDataStep = -4;
    }
    else { /* even lines */
      dir = TO_RIGHT;
      xstart = 0; xstop = _sizeX; xstep = 1; iterDataStep = 4;
    } /* if (TEST_ODD(y)) */

    iterData    = (unsigned char*)&_data[y*_sizeX+xstart];
    iterOutData = (unsigned char*)&_outData[y*_sizeX+xstart];

    for (int x = xstart; x != xstop; x += xstep)
    {
      input = 255;
      input += *(iterData);
      input += *(iterData+1);
      input += *(iterData+2);
      input >>= 2;

      corrected_level = input + _carryLine0[x];
      if (corrected_level <= threshold)
        intensity = BLACK; /* put black */
      else
        intensity = WHITE; /* put white */
      diff = corrected_level - intensity;
      distribute_error(x, y, diff, dir, input);

      if (input == BLACK || intensity == BLACK) 
        memset(iterOutData, BLACK, sizeof(RGBA));
      else
        memset(iterOutData, WHITE, sizeof(RGBA));

      iterData += iterDataStep;
      iterOutData += iterDataStep;
      
    } /* x-cycle */
    shift_carry_buffers();
  } /* y-cycle */
  
}

