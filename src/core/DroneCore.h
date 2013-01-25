/*
 *  DroneCore.h
 *  drone
 *
 *  Created by foogy on 22/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DRONE_CORE_INC
#define DRONE_CORE_INC

#include <QString>

class DroneCore 
{
public:
	static void init();
	static void release();
  QString static newUUID();
};

#endif
