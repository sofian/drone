/* Gear_VideoOutput.h
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

#ifndef GEAR_VIDEOOUTPUT_INCLUDED
#define GEAR_VIDEOOUTPUT_INCLUDED

#include "Gear.h"
#include "TextureType.h"



class DroneQGLWidget;
class DroneGLWindow;

class Gear_VideoOutput : public Gear  
{
public:

  Gear_VideoOutput();
  virtual ~Gear_VideoOutput();

  void runVideo();

  PlugIn<TextureType>* VIDEO_IN(){return _VIDEO_IN;};
  
  bool isFullscreen();

protected:
  void internalInit();  
  void onUpdateSettings();
  void internalPrePlay();


private:
  static const QString SETTING_XRES;
  static const QString SETTING_YRES;
  static const QString SETTING_XPOS;
  static const QString SETTING_YPOS;

  static const QString SETTING_FULLSCREEN;

  static const int DEFAULT_XRES;
  static const int DEFAULT_YRES;    
  static const int DEFAULT_XPOS;
  static const int DEFAULT_YPOS;    
	
  static const bool DEFAULT_FULLSCREEN; 

  PlugIn<TextureType> *_VIDEO_IN;
  DroneQGLWidget *_droneQGLWidget;
  DroneGLWindow *_window;
  TextureType _textureCopy;
};

#endif
