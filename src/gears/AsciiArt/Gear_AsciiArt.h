/* Gear_AsciiArt.h
 * Copyright (C) 2004 Mathieu Petit-Clair
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


#ifndef GEAR_ASCIIART_INCLUDED
#define GEAR_ASCIIART_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "EnumType.h"
#include "ValueType.h"

#include <aalib.h>

class Gear_AsciiArt : public Gear
{

  enum eRenderingSpeed
  {
    FAST,         // 0 
    GOOD,
    N_RENDERING_TYPES
  };

  enum eDitheringType
  {
    NONE,         // 0 
    ERROR,
    FLOYD,
    N_DITHERING_TYPES
  };

  enum eCharSet
  {
    BASIC,         // 0 
    EXTENDED,
    N_CHARSET_TYPES
  };

public:
  Gear_AsciiArt(Schema *schema, std::string name);
  virtual ~Gear_AsciiArt();

  void init();
  
  void runVideo();

  bool ready();

  void onUpdateSettings();

private:
  
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<EnumType> *_RENDER_IN;
  PlugIn<EnumType> *_DITHER_IN;
  PlugIn<EnumType> *_CHAR_IN;
  PlugIn<ValueType> *_CONTRAST_IN;
  PlugIn<ValueType> *_BRIGHTNESS_IN;

  const VideoRGBAType *_inImage; 
  VideoRGBAType *_outImage;
  unsigned char *_outData;

  int _sizeY;
  int _sizeX;
  int _size;
  unsigned char *_inData;

  aa_context *_aaContext;
  unsigned char *_aaFramebuffer;
  aa_hardware_params _aaHDParams;
 
  eRenderingSpeed _renderingSpeed;
  eDitheringType _ditheringType;
  eCharSet _charSet;
  
  int _contrast;
  int _brightness;

};

#endif
