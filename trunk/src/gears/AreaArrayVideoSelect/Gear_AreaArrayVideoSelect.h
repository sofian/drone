/* Gear_AreaArrayVideoSelect.h
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

#ifndef GEAR_AREAARRAYMASK_INCLUDED
#define GEAR_AREAARRAYMASK_INCLUDED


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
class Gear_AreaArrayVideoSelect : public Gear
{
public:
  //! Default constructor.
  Gear_AreaArrayVideoSelect(Schema *schema, std::string uniqueName);

  //! Destructor.
  virtual ~Gear_AreaArrayVideoSelect();

  void runVideo();

private:
  //! Output video list.
  PlugOut<ListType> *_VIDEO_LIST_OUT;

  //! Array of areas.
  PlugIn<AreaArrayType> *_SELECT_IN;

  //! Input video.
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  
  // Internal use.
  const VideoRGBAType *_imageIn;
  const AreaArrayType *_areaArray;
  ListType *_listOut;
  VideoRGBAType *_imageOut;
};

#endif
