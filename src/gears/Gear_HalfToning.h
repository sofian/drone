#ifndef GEAR_HALFTONING_INCLUDED
#define GEAR_HALFTONING_INCLUDED

/*
  Version 1 (August 2001), as described in the article
  "A Simple and Efficient Error-Diffusion Algorithm" (SIGGRAPH'01)
   Author: Victor Ostromoukhov
   University of Montreal, http://www.iro.umontreal.ca/~ostrom/

*******************************************************************/


#include "Gear.h"
#include "VideoType.h"

#include <stdio.h>
#include <math.h>

struct ThreeCoefficients
{
  float i_r;        /* right */
  float i_dl;       /* down-left */
  float i_d;        /* down */
  float i_sum;      /* sum */
//   int i_r;        /* right */
//   int i_dl;       /* down-left */
//   int i_d;        /* down */
//   int i_sum;      /* sum */
};

class Gear_HalfToning : public Gear
{
public:

  Gear_HalfToning(Engine *engine, std::string name);
  virtual ~Gear_HalfToning();

  void runVideo();  bool ready();

private:
  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  //local var
  MatrixType<RGBA> *_image; 
  MatrixType<RGBA> *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _sizeY;
  int _sizeX;    

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  RGBAfloat *_carryLine0; /* carry buffer; current line     */
  RGBAfloat *_carryLine1; /* carry buffer; current line + 1 */

};


#endif
