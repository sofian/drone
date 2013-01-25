/*
 *  DroneCore.cpp
 *  drone
 *
 *  Created by foogy on 22/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "DroneCore.h"

#include <QString>

#include "gearFactory/GearMaker.h"
#include "DroneMath.h"
#include "Utils.h"

extern Gear* makeGear_PushButton();

extern "C"
{
#ifdef WIN32
#include <Rpc.h>
#else
#include <linux/uuid.h>
#endif
}

namespace Drone{
    LoadingModeFlags LoadFromFile=0; 
    LoadingModeFlags PasteFromClipboard=0;
    LoadingModeFlags RestoreSnapshot = QFlags<LoadingModeFlag>(UpdateWhenPossible | DeleteUnvisitedElements | EmitSignals);
}

QString DroneCore::newUUID()
{
#ifdef WIN32
    UUID uuid;
    UuidCreate ( &uuid );

    unsigned char * str;
    UuidToStringA ( &uuid, &str );

    std::string s( ( char* ) str );

    RpcStringFreeA ( &str );
#else
//    uuid_t uuid;
//    uuid_generate_random ( uuid );
    char s[37];
//    uuid_unparse ( uuid, s );
#endif
    return s;
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

