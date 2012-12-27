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
#include "DroneQGLWidget.h"
#include "DroneGLWindow.h"

#include "GearMaker.h"
#include "Engine.h"


#include <iostream>

#include <qapplication.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <Q3HBoxLayout>
#include <QPaintEvent>

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
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

const std::string Gear_VideoOutput::SETTING_XRES = "XRes";
const std::string Gear_VideoOutput::SETTING_YRES = "YRes";

const std::string Gear_VideoOutput::SETTING_XPOS = "XPos";
const std::string Gear_VideoOutput::SETTING_YPOS = "YPos";

const std::string Gear_VideoOutput::SETTING_FULLSCREEN = "FullScreen";


Gear_VideoOutput::Gear_VideoOutput(Schema *schema, std::string uniqueName) : 
  Gear(schema, "VideoOutput", uniqueName),
  _droneQGLWidget(NULL),
  _window(NULL),
  _frameSizeX(0),
  _frameSizeY(0)
{

  addPlug(_VIDEO_IN = new PlugIn<TextureType>(this, "IN", true));

  _settings.add(Property::INT, SETTING_XRES)->valueInt(DEFAULT_XRES);
  _settings.add(Property::INT, SETTING_YRES)->valueInt(DEFAULT_YRES);
  _settings.add(Property::INT, SETTING_XPOS)->valueInt(DEFAULT_XPOS);
  _settings.add(Property::INT, SETTING_YPOS)->valueInt(DEFAULT_YPOS);

  _settings.add(Property::BOOL, SETTING_FULLSCREEN)->valueBool(DEFAULT_FULLSCREEN);

}

Gear_VideoOutput::~Gear_VideoOutput()
{
  //todo mg: delete qglwiget too
  _window->deleteLater();
}

void Gear_VideoOutput::onUpdateSettings()
{	
	_window->move(_settings.get(SETTING_XPOS)->valueInt(), _settings.get(SETTING_YPOS)->valueInt());
  
	if (_settings.get(SETTING_FULLSCREEN)->valueBool())
		_window->showFullScreen();
	else
		_window->showNormal();
}

void Gear_VideoOutput::internalInit()
{ 
  std::cout << "--==|| GL output initialization ||==--" << std::endl;
  
  if (_window)
    delete _window;
  
  _window = new DroneGLWindow(qApp->mainWidget());
  _droneQGLWidget = new DroneQGLWidget(_window, this);
  Q3BoxLayout *l = new Q3HBoxLayout(_window);
  l->addWidget(_droneQGLWidget);
  _window->setModal(false);
  _window->show();
}

void Gear_VideoOutput::internalPrePlay()
{
	_window->move(_settings.get(SETTING_XPOS)->valueInt(), _settings.get(SETTING_YPOS)->valueInt());
  
	if (_settings.get(SETTING_FULLSCREEN)->valueBool())
		_window->showFullScreen();
	else
		_window->showNormal();
}

void Gear_VideoOutput::runVideo()
{
  if (!_VIDEO_IN->connected())
    return;

  if (_VIDEO_IN->type()->textureName() == 0)
    return;

  if (_frameSizeX!=_VIDEO_IN->type()->textureSizeX() || _VIDEO_IN->type()->textureSizeX())
  {
    _frameSizeX = _VIDEO_IN->type()->textureSizeX();
    _frameSizeY = _VIDEO_IN->type()->textureSizeY();
  }
  
  _droneQGLWidget->setCurrentTexture(_VIDEO_IN->type());
  
  //asynchronously tell the widget to repaint himself in a thread-safe way
  QApplication::postEvent(_droneQGLWidget, new QPaintEvent( QRect(0, 0, _frameSizeX, _frameSizeY) ) );
}

bool Gear_VideoOutput::isFullscreen() const
{
  return _settings.get(SETTING_FULLSCREEN)->valueBool();
}