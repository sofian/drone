/*
 *  DroneCore.cpp
 *  drone
 *
 *  Created by foogy on 22/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "DroneCore.h"
#include <QUuid>
#include "gearFactory/GearMaker.h"
#include "DroneMath.h"
#include "Utils.h"

extern Gear* makeGear_PushButton();


namespace Drone{
    LoadingModeFlags LoadFromFile=0; 
    LoadingModeFlags PasteFromClipboard=0;
    LoadingModeFlags RestoreSnapshot = QFlags<LoadingModeFlag>(UpdateWhenPossible | DeleteUnvisitedElements | EmitSignals);
}

QString DroneCore::newUUID()
{
  return QUuid::createUuid().toString();
}

void DroneCore::init()
{
	std::cout << "initializing the drone core..." << std::endl;
	
  GearMaker::instance()->registerStaticGear(&makeGear_PushButton);
	GearMaker::instance()->parse();
	
  initMath();
  paint_funcs_setup();
}

void DroneCore::release()
{
	paint_funcs_free();
}

