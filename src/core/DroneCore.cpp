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

#include "DroneMath.h"
#include "Utils.h"

extern Gear* makeGear_PushButton();


void DroneCore::init()
{
	std::cout << "initializing the drone core..." << std::endl;
	GearMaker::instance()->parse();
	
  GearMaker::instance()->registerStaticGear("PushButton", &makeGear_PushButton);
	
  
  initMath();
  paint_funcs_setup();
	
}
void DroneCore::release()
{
	paint_funcs_free();
}
