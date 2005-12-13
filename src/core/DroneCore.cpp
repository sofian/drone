/* ConnectionItem.h
* Copyright (C) 2005 Mathieu Guindon, Julien Keable
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

#include "DroneCore.h"

#include "GearMaker.h"

#include "MathUtil.h"
#include "Utils.h"

void DroneCore::init()
{
	std::cout << "initializing the drone core..." << std::endl;
	GearMaker::parseGears();
	
  initMath();
  paint_funcs_setup();
	
}

void DroneCore::release()
{
	paint_funcs_free();
}
