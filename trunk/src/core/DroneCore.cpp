/*
 *  DroneCore.cpp
 *  drone
 *
 *  Created by foogy on 22/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "DroneCore.h"

#include "GearMaker.h"
#include "MetaGearMaker.h"

#include "Math.h"
#include "Utils.h"

void DroneCore::init()
{
	std::cout << "initializing the drone core..." << std::endl;
	GearMaker::parseGears();
	MetaGearMaker::parseMetaGears();
	
  initMath();
  paint_funcs_setup();
	
}

void DroneCore::release()
{
	paint_funcs_free();
}
