/* Gear_PackList.h
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

#ifndef GEAR_PACKLIST_INCLUDED
#define GEAR_PACKLIST_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ListType.h"


class Gear_PackList : public Gear  
{
public:
  Gear_PackList();
  virtual ~Gear_PackList();

  void runVideo();

protected:

  void internalPrePlay();

  void clearList();
		
  PlugIn<AbstractType> *_ELEM_IN;

  PlugIn<ListType> *_LIST_IN;	
  PlugOut<ListType> *_LIST_OUT;
	
};

#endif 
