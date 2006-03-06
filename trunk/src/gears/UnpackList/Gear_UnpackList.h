/* Gear_UnpackList.h
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

#ifndef GEAR_UNPACKLIST_INCLUDED
#define GEAR_UNPACKLIST_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ValueType.h"
#include "EnumType.h"
#include "VideoRGBAType.h"
#include "VideoChannelType.h"
#include "AreaType.h"
#include "ListType.h"


class Gear_UnpackList : public Gear  
{
public:
  Gear_UnpackList(Schema *schema, std::string uniqueName);
  virtual ~Gear_UnpackList();

  void runVideo();

protected:

  void internalPrePlay();

  void clearList();
		
  PlugIn<ListType> *_LIST_IN;	
  PlugOut<ListType> *_LIST_OUT;

  PlugOut<StringType> *_STR_OUT;
  PlugOut<ValueType> *_VAL_OUT;
  PlugOut<EnumType> *_ENUM_OUT;
  PlugOut<VideoChannelType> *_CHANNEL_OUT;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<AreaType> *_AREA_OUT;
};

#endif 
