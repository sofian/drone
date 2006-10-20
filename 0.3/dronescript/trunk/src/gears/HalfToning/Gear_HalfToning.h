/* Gear_HalfToning.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GEAR_HALFTONING_INCLUDED
#define GEAR_HALFTONING_INCLUDED

#include "Gear.h"
#include "VideoRGBAType.h"

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
  Gear_HalfToning(Schema *schema, std::string uniqueName);

  // Destructor.
  virtual ~Gear_HalfToning();

  void runVideo();
private:
  //! Video input.
  PlugIn<VideoRGBAType> *_VIDEO_IN;

  //! Video output.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  // Internal use.
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
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
