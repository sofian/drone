/* Gear_VideoOutput.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#include "Gear_VideoOutput.h"

#include "GearMaker.h"
#include "Engine.h"
#include "VideoOutput.h"
#include "VideoOutputMaker.h"

#include <iostream>

Register_Gear(MAKERGear_VideoOutput, Gear_VideoOutput, "VideoOutput")

const int Gear_VideoOutput::DEFAULT_XRES = 352;
const int Gear_VideoOutput::DEFAULT_YRES = 240;
const bool Gear_VideoOutput::DEFAULT_FULLSCREEN = false; 

const std::string Gear_VideoOutput::SETTING_XRES = "XRes";
const std::string Gear_VideoOutput::SETTING_YRES = "YRes";
const std::string Gear_VideoOutput::SETTING_FULLSCREEN = "FullScreen";


Gear_VideoOutput::Gear_VideoOutput(Engine *engine, std::string name) : 
Gear(engine, "VideoOutput", name),
_videoOutput(NULL)
{
  //populate available video output list in order of preference
  //the init will try them in order, until he find one that fit
  _allOutputs.push_back("Gl");
  _allOutputs.push_back("Gl");
  _allOutputs.push_back("Shm");
  //

  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, name));

  _settings.add(Property::INT, SETTING_XRES)->valueInt(DEFAULT_XRES);
  _settings.add(Property::INT, SETTING_YRES)->valueInt(DEFAULT_YRES);
  _settings.add(Property::BOOL, SETTING_FULLSCREEN)->valueBool(DEFAULT_FULLSCREEN);

}

Gear_VideoOutput::~Gear_VideoOutput()
{    
  delete _videoOutput;
}


bool Gear_VideoOutput::ready()
{
  return _VIDEO_IN->connected();
}

void Gear_VideoOutput::init()
{             
  std::cout << "selecting best video output for your hardware..." << std::endl; 
  for (std::vector<std::string>::iterator it=_allOutputs.begin();it!=_allOutputs.end();++it)
  {
    std::cout << "trying " << (*it) << "..." << std::endl;
    if ((_videoOutput = VideoOutputMaker::makeVideoOutput(*it))!=NULL)
    {
      if (_videoOutput->init(_settings.get(SETTING_XRES)->valueInt(), _settings.get(SETTING_YRES)->valueInt(), false))
      {
        std::cout << (*it) << " is perfect for you!" << std::endl;
        return;
      } else
      {
        std::cout << (*it) << " is not what you need" << std::endl;
        delete _videoOutput;
        _videoOutput=NULL;
      }
    } else
      std::cout << (*it) << " not available" << std::endl;

  }

  std::cout << "sac a papier! fail to find a video output!!!" << std::endl;
}

void Gear_VideoOutput::prePlay()
{

}

void Gear_VideoOutput::postPlay()
{
}


void Gear_VideoOutput::runVideo()
{       
  if (!_VIDEO_IN->connected() || _videoOutput==NULL)
    return;

  if (_videoOutput==NULL)
    return;

  if (_VIDEO_IN->type()->width()==0 || _VIDEO_IN->type()->height()==0)
    return;

  _videoOutput->render(*(_VIDEO_IN->type()));
}






