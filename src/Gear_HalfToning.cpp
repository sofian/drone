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
  for (int i=0; i<256; ++i)
    // *** en fait le mieux serait de les calculer directo... on a juste besoin d'un tableau pour r et dl (paires)
  {
    COEFS_TABLE[i].i_r = COEFS_TABLE[i].i_r / COEFS_TABLE[i].i_sum;
    COEFS_TABLE[i].i_dl = COEFS_TABLE[i].i_dl / COEFS_TABLE[i].i_sum;
  }
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
//   if (_carryLine0)
//     _carryLine0-=1;
//   if (_carryLine1)
//     _carryLine1-=1;
  
  _carryLine0 = (RGBAfloat*)realloc(_carryLine0, (_sizeX+2)*sizeof(RGBAfloat));
  _carryLine1 = (RGBAfloat*)realloc(_carryLine1, (_sizeX+2)*sizeof(RGBAfloat));
  memset(_carryLine0, 0, (_sizeX+2)*sizeof(RGBAfloat));
  memset(_carryLine1, 0, (_sizeX+2)*sizeof(RGBAfloat));
//   _carryLine0+=1;
//   _carryLine1+=1;
  
  unsigned char *iterData;
  unsigned char *iterOutData;
  float *iterCarryLine0;
  float *iterCarryLine1;

  int xstart, xstop, xstep, dir, iterDataStep;
  int input, intensity;
  float threshold = 127.5, corrected_level, diff;

  float term_r, term_dl, term_d;
  ThreeCoefficients coefs;

  for (int y = 0; y < _sizeY; ++y)
  {
    if (y & 1)
    {
      // odd line
      dir = -4;
      xstart = _sizeX-1; xstop = -1; xstep = -1; iterDataStep = -7;
    }
    else
    {
      // even line
      dir = 4;
      xstart = 0; xstop = _sizeX; xstep = 1; iterDataStep = 1;
    }

    iterData    = (unsigned char*)&_data[y*_sizeX+xstart];
    iterOutData = (unsigned char*)&_outData[y*_sizeX+xstart];

    iterCarryLine0 = (float*)&_carryLine0[xstart+1];
    iterCarryLine1 = (float*)&_carryLine1[xstart+1];
    
    for (int x = xstart; x != xstop; x += xstep)
    {
      for (int z=0; z<3; ++z)
      {
        input = (int) *iterData;

        // Correct level.
        corrected_level = input + *iterCarryLine0;

        // Compute intensity.
        intensity = (corrected_level > threshold ? 255 : 0);

        // Distribute error.

        diff = corrected_level - intensity;
        
        coefs = COEFS_TABLE[input];

        term_r = coefs.i_r*diff;
        term_dl = coefs.i_dl*diff;
        term_d = diff - (term_r+term_dl);
        
        *(iterCarryLine0+dir) += term_r;
        *(iterCarryLine1-dir) += term_dl;
        *(iterCarryLine1)     += term_d;

         // Put color.
        *iterOutData = (input & intensity ? 255 : 0);

        ++iterData;
        ++iterOutData;
        ++iterCarryLine0;
        ++iterCarryLine1;
      }
      
      iterData       += iterDataStep;
      iterOutData    += iterDataStep;
      iterCarryLine0 += iterDataStep;
      iterCarryLine1 += iterDataStep;
    }

    // Shift carry buffers.
    RGBAfloat *tmp = _carryLine0;
    _carryLine0 = _carryLine1;
    _carryLine1 = tmp;
    memset(_carryLine1, 0, (_sizeX+2)*sizeof(RGBAfloat));
    
  }
  
}

