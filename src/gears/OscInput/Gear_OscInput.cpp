/* Gear_OscInput.cpp
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


#include "Gear_OscInput.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_OscInput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "OscInput";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_OscInput::Gear_OscInput(Schema *schema, std::string uniqueName) : 
  Gear(schema, "OscInput", uniqueName),
  _forceOscServerInit(true),
  _currentPort(""),
  _loServerThread(NULL)
{

  addPlug(_PORT = new PlugIn<StringType>(this, "Port", false, new StringType("7770")));

  addPlug(_OSC_OUT = new PlugOut<OscMessageType>(this, "Osc Out", false));
}

Gear_OscInput::~Gear_OscInput()
{

}

void Gear_OscInput::runAudio()
{
  if (_forceOscServerInit ||
       _currentPort != _PORT->type()->value())
  {
     _currentPort = _PORT->type()->value();
     _forceOscServerInit=false;
     startOscServer(_currentPort);
  }
}

void Gear_OscInput::internalPostPlay()
{
	stopOscServer();
	_forceOscServerInit=true;//for the next startup
}

void Gear_OscInput::startOscServer(std::string port)
{
	stopOscServer();
	
	_loServerThread = lo_server_thread_new(port.c_str(), error);
	if (!_loServerThread)
	{
		std::cout << "fail to start osc server thread!" << std::endl;
		return;
	}
		
	//add the method that will handle osc message
	lo_server_thread_add_method(_loServerThread, NULL, NULL, configuredOscHandler, this);
		
	lo_server_thread_start(_loServerThread);
	
}

void Gear_OscInput::stopOscServer()
{
	if (!_loServerThread)
		return;
	
	lo_server_thread_free(_loServerThread);
	_loServerThread=NULL;
}

int Gear_OscInput::configuredOscHandler(const char *path, const char *types, lo_arg **argv, int argc,
																				void *data, void *user_data)
{
	std::cout << "Osc message received : " << std::endl;
	std::cout << "path: " << path << std::endl;
	for (int i=0; i<argc; i++) 
	{
		std::cout << "arg " << i << " " << types[i] << std::endl;
	}	
	
	return 0;
}

void Gear_OscInput::error(int num, const char *m, const char *path)
{
	std::cout << "liblo server error " << num << " in path " << m << ": " << path << std::endl;
}
