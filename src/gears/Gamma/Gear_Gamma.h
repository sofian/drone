/* Gear_Gamma.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_GAMMA_INCLUDED
#define GEAR_GAMMA_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"


class Gear_Gamma : public Gear
{
public:

  Gear_Gamma();
  virtual ~Gear_Gamma();

  void runVideo();
  
  bool ready();

protected:
  void internalInit();

private:
  // Recompute look-up-table based on current gamma level.
  void computeLookUpTable();

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_GAMMA_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  // Lookup table for gamma values.
  unsigned char _lut[256];

  float _gamma; // the current amount of gamma

  int _size;

  const unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
