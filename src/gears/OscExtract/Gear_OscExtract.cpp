/* Gear_OscExtract.cpp
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


#include "Gear_OscExtract.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_OscExtract(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "OscExtract";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_OscExtract::Gear_OscExtract(Schema *schema, std::string uniqueName) :
	Gear(schema, "OscExtract", uniqueName)
{
	addPlug(_OSC_IN = new PlugIn<OscMessageType>(this, "Osc In", false));
	addPlug(_PATH_OUT = new PlugOut<StringType>(this, "path", false));
  addPlug(_ARGS_OUT = new PlugOut<ListType>(this, "args", false));
}

Gear_OscExtract::~Gear_OscExtract()
{

}

void Gear_OscExtract::runVideo()
{
//  std::cout << "extracting: " << _OSC_IN->type()->path().value() << ", size = " << _OSC_IN->type()->args().size() << std::endl;
  _PATH_OUT->type()->setValue(_OSC_IN->type()->path().value() );

  const ListType& args = _OSC_IN->type()->args();
  if (args.size()>0) {
    //std::cout << " ---> " << ((ValueType*)_OSC_IN->type()->args()[0])->value() << std::endl;
    _ARGS_OUT->type()->clear();
    for (ListType::const_iterator it = args.begin(); it != args.end(); ++it)
   // for (int i=0; i<args.size(); i++)
      _ARGS_OUT->type()->push_back( *it );

    //std::copy( _OSC_IN->type()->args().begin(), _OSC_IN->type()->args().end(), _ARGS_OUT->type()->begin() );
    //_ARGS_OUT->type()->assign( _OSC_IN->type()->args().begin(), _OSC_IN->type()->args().end() );
  }
}
