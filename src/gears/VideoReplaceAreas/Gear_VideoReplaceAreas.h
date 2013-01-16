/* Gear_VideoReplaceAreas.h
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

#ifndef GEAR_VIDEOREPLACEAREAS_INCLUDED
#define GEAR_VIDEOREPLACEAREAS_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "AreaArrayType.h"
#include "ListType.h"

/**
 * This gear creates an output mask from an array of area (AreaArrayType).
 * The mask is false (black) in every point where no area falls and is true
 * (white) wherever lies an area.
 *
 * @see AreaArrayType
 * @author Jean-S�bastien Sen�cal
 * @version %I% %G%
 */
class Gear_VideoReplaceAreas : public Gear
{
public:
  //! Default constructor.
  Gear_VideoReplaceAreas();

  //! Destructor.
  virtual ~Gear_VideoReplaceAreas();

  void runVideo();

private:
  //! Output video.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  //! Input video.
  PlugIn<VideoRGBAType> *_VIDEO_IN;

  //! Input video list.
  PlugIn<ListType> *_VIDEO_LIST_IN;

  //! Array of areas.
  PlugIn<AreaArrayType> *_REPLACE_IN;

  // Internal use.
  VideoRGBAType *_imageOut;
  const VideoRGBAType *_imageIn;
  
  const AreaArrayType *_areaArray;
  const ListType *_listIn;

  const VideoRGBAType *_subImageIn;
};

#endif
