/* Gear_AreaArrayClip.h
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

#ifndef GEAR_AREAARRAYCLIP_INCLUDED
#define GEAR_AREAARRAYCLIP_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "AreaArrayType.h"
#include "ListType.h"
//#include "ValueType.h"
#include "error.h"

/**
 * This gear creates an output mask from an array of area (AreaArrayType).
 * The mask is false (black) in every point where no area falls and is true
 * (white) wherever lies an area.
 *
 * @see AreaArrayType
 * @author Jean-Sébastien Senécal
 * @version %I% %G%
 */
class Gear_AreaArrayClip : public Gear
{
public:
  //! Default constructor.
  Gear_AreaArrayClip(Schema *schema, std::string uniqueName);

  //! Destructor.
  virtual ~Gear_AreaArrayClip();

  void runVideo();

private:
  //! Output area array.
  PlugOut<AreaArrayType> *_AREA_ARRAY_OUT;

  //! Intput area array.
  PlugIn<AreaArrayType> *_AREA_ARRAY_IN;

  //! Input video.
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  
  // Internal use.
  const AreaArrayType *_areaArrayIn;
  AreaArrayType *_areaArrayOut;
  int _sizeX, _sizeY;
};

#endif
