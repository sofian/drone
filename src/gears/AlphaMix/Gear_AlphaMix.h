/* Gear_AlphaMix.h
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#ifndef GEAR_ALPHAMIX_INCLUDED
#define GEAR_ALPHAMIX_INCLUDED

#include "Gear.h"
#include "ValueType.h"
#include "EnumType.h"
#include "VideoRGBAType.h"
#include "Utils.h"
#include "AlphaComposite.h"

class Gear_AlphaMix : public Gear
{
  enum eAlphaMixType
  {
    OVER,
    IN,
    OUT,
    ATOP,
    XOR,
    N_ALPHAMIX_TYPES
  };

public:

  Gear_AlphaMix(Schema *schema, std::string uniqueName);
  virtual ~Gear_AlphaMix();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN_A;
  PlugIn<VideoRGBAType> *_VIDEO_IN_B;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<EnumType> *_MIXFUNC_IN;

  //local var
  const VideoRGBAType *_imageA; 
  const VideoRGBAType *_imageB; 
  VideoRGBAType *_outImage; 
//   const RGBA *_dataA;
//   const RGBA *_dataB;
//   RGBA *_outData;
//   const RGBA *_largerImage;
//   const RGBA *_tallerImage;

  const unsigned char *_imageInA;
  const unsigned char *_imageInB;
  unsigned char *_imageOut;

  int _iterSizeY;
  int _iterSizeX;    
  int _imageASizeX;
  int _imageASizeY;
  int _imageBSizeX;
  int _imageBSizeY;
  int _imageOutSizeX;
  int _imageOutSizeY;

  eAlphaMixType _mixType;

//   double *_mmxImageInA;
//   double *_mmxImageInB;
//   double *_mmxImageOut;

//   int _remainIterSizeY;
//   int _remainIterSizeX;


};

#endif
