/* Gear_VideoMix.h
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

#ifndef GEAR_VIDEOMIX_INCLUDED
#define GEAR_VIDEOMIX_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "Utils.h"

class Gear_VideoMix : public Gear
{
  enum eVideoMixType
  {
    BLEND,            // 0
    SHADE,
    DARKEN,
    LIGHTEN,
    HUE_ONLY,
    SATURATION_ONLY,  // 5
    VALUE_ONLY,
    COLOR_ONLY,
    MULTIPLY,
    DIVIDE,
    SCREEN,           // 10
    OVERLAY,
    DODGE,
    BURN,
    HARDLIGHT,
    SOFTLIGHT,        // 15
    GRAIN_EXTRACT,
    GRAIN_MERGE,
    ADD,
    SUBTRACT,
    DIFFERENCE,       // 20
    //    DISSOLVE, // XXX trop de paramètres, faire dans une gear differente
    //    REPLACE,  // XXX idem
    N_VIDEOMIX_TYPES
  };
  static const std::string SETTING_MIX_FUNCTION;

public:

  Gear_VideoMix(Engine *engine, std::string name);
  virtual ~Gear_VideoMix();

  void runVideo();

  PlugIn<VideoRGBAType>* VIDEO_IN_A(){return _VIDEO_IN_A;}
  PlugIn<VideoRGBAType>* VIDEO_IN_B(){return _VIDEO_IN_B;}
  PlugOut<VideoRGBAType>* VIDEO_OUT(){return _VIDEO_OUT;}  bool ready();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN_A;
  PlugIn<VideoRGBAType> *_VIDEO_IN_B;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;
  PlugIn<ValueType> *_MIXFUNC_IN;

  //local var
  const VideoRGBAType *_imageA; 
  const VideoRGBAType *_imageB; 
  VideoRGBAType *_outImage; 
  const RGBA *_dataA;
  const RGBA *_dataB;
  RGBA *_outData;
  const RGBA *_largerImage;
  const RGBA *_tallerImage;

  int _iterSizeY;
  int _iterSizeX;    
  int _imageASizeX;
  int _imageASizeY;
  int _imageBSizeX;
  int _imageBSizeY;
  int _imageOutSizeX;
  int _imageOutSizeY;

  eVideoMixType _mixType;

  double *_mmxImageInA;
  double *_mmxImageInB;
  double *_mmxImageOut;

  int _remainIterSizeY;
  int _remainIterSizeX;


};

#endif
