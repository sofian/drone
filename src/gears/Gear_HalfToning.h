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

// Internal use.
struct ThreeCoefficients
{
  float i_r;        /* right */
  float i_dl;       /* down-left */
  float i_d;        /* down */
  float i_sum;      /* sum */
};

/**
 * This class implements Victor Ostromoukhov error-diffusion algorithm, as
 * described in A Simple and Efficient Error-Diffusion Algorithm" (SIGGRAPH'01).
 *
 * @author Jean-Sébastien Senécal, Victor Ostromoukhov
 * @version %I% %G%
 */
class Gear_HalfToning : public Gear
{
  
public:
  //! Default constructor.
  Gear_HalfToning(Engine *engine, std::string name);

  // Destructor.
  virtual ~Gear_HalfToning();

  void runVideo();
  bool ready();

private:
  //! Video input.
  PlugIn<VideoTypeRGBA> *_VIDEO_IN;

  //! Video output.
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  // Internal use.
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 
  const RGBA *_data;
  RGBA *_outData;

  int _sizeY;
  int _sizeX;    

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  // Carry buffer, current line.
  RGBAfloat *_carryLine0;
  
  // Carry buffer, next line.
  RGBAfloat *_carryLine1;

};


#endif
