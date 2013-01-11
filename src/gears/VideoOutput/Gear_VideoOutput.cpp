/* Gear_VideoOutput.cpp
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

#include "Gear_VideoOutput.h"

#include "GearMaker.h"
#include "Engine.h"
#include "VideoOutput.h"

#if defined(Q_OS_MACX)
#include "VideoOutputGl.h"
#else
#include "VideoOutputMaker.h"
#endif

#include <iostream>

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_VideoOutput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoOutput";
  gearInfo.classification = GearClassifications::video().IO().instance();
  return gearInfo;
}
}
  
const int Gear_VideoOutput::DEFAULT_XRES = 352;
const int Gear_VideoOutput::DEFAULT_YRES = 240;
const int Gear_VideoOutput::DEFAULT_XPOS = 0;
const int Gear_VideoOutput::DEFAULT_YPOS = 0;

const bool Gear_VideoOutput::DEFAULT_FULLSCREEN = false; 

const QString Gear_VideoOutput::SETTING_XRES = "XRes";
const QString Gear_VideoOutput::SETTING_YRES = "YRes";

const QString Gear_VideoOutput::SETTING_XPOS = "XPos";
const QString Gear_VideoOutput::SETTING_YPOS = "YPos";

const QString Gear_VideoOutput::SETTING_FULLSCREEN = "FullScreen";


Gear_VideoOutput::Gear_VideoOutput(Schema *schema, QString uniqueName) : 
Gear(schema, "VideoOutput", uniqueName),
_videoOutput(NULL)
{
  //populate available video output list in order of preference
  //the init will try them in order, until he find one that fit
#ifndef Q_OS_MACX
  //_allOutputs.push_back("Xv");
  _allOutputs.push_back("Gl");
  _allOutputs.push_back("Shm");
  _allOutputs.push_back("QT");
#endif  
  //

  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "IN", true));

  _settings.add(Property::INT, SETTING_XRES)->valueInt(DEFAULT_XRES);
  _settings.add(Property::INT, SETTING_YRES)->valueInt(DEFAULT_YRES);
  _settings.add(Property::INT, SETTING_XPOS)->valueInt(DEFAULT_XPOS);
  _settings.add(Property::INT, SETTING_YPOS)->valueInt(DEFAULT_YPOS);

  _settings.add(Property::BOOL, SETTING_FULLSCREEN)->valueBool(DEFAULT_FULLSCREEN);

}

Gear_VideoOutput::~Gear_VideoOutput()
{    
  delete _videoOutput;
}

void Gear_VideoOutput::onUpdateSettings()
{	
  _videoOutput->toggleFullscreen(_settings.get(SETTING_FULLSCREEN)->valueBool(), 1024, 768, _settings.get(SETTING_XPOS)->valueInt(), _settings.get(SETTING_YPOS)->valueInt());
}

void Gear_VideoOutput::internalInit()
{ 
//osx version dont use the VideoOutputMaker strategy and directly use the GL Output
#if defined(Q_OS_MACX)
	_videoOutput = new VideoOutputGl();
	if (!_videoOutput->init(_settings.get(SETTING_XRES)->valueInt(), _settings.get(SETTING_YRES)->valueInt(), false))
	{
		std::cout << "fail to init GL videoOutput" << std::endl;
		delete _videoOutput;
		_videoOutput=NULL;
	}
#else
  std::cout << "selecting best video output for your hardware..." << std::endl; 
  for (std::vector<QString>::iterator it=_allOutputs.begin();it!=_allOutputs.end();++it)
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
#endif
}

void Gear_VideoOutput::internalPrePlay()
{
  _videoOutput->toggleFullscreen(_settings.get(SETTING_FULLSCREEN)->valueBool(), 1024, 768, _settings.get(SETTING_XPOS)->valueInt(), _settings.get(SETTING_YPOS)->valueInt());
}

void Gear_VideoOutput::runVideo()
{
  if (!_VIDEO_IN->connected() || _videoOutput==NULL)
    return;

  if (_videoOutput==NULL)
    return;

  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _videoOutput->render(*_image);
}






