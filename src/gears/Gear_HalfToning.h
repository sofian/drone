#ifndef GEAR_HALFTONING_INCLUDED
#define GEAR_HALFTONING_INCLUDED

#include "Gear.h"
#include "VideoType.h"

#include <stdio.h>
#include <math.h>

/**
 * This class implements Victor Ostromoukhov error-diffusion algorithm, as
 * described in A Simple and Efficient Error-Diffusion Algorithm" (SIGGRAPH'01).
 * The code could be found on the author's personal web page: 
 * http://www.iro.umontreal.ca/~ostrom/
 *
 * @author Jean-Sébastien Senécal, Victor Ostromoukhov
 * @version %I% %G%
 */
class Gear_HalfToning : public Gear
{
  // Internal use.
  struct ThreeCoefficients
  {
    float i_r;        /* right */
    float i_dl;       /* down-left */
  };

  // Internal use.
  static const ThreeCoefficients COEFS_TABLE[256];
  
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
