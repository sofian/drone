/* Gear_Frei0r.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef GEAR_FREI0R_INCLUDED
#define GEAR_FREI0R_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"

#include "frei0r.h"

class Gear_Frei0r : public Gear
{
public:

  Gear_Frei0r(Engine *engine, std::string name);
  virtual ~Gear_Frei0r();

  void runVideo();  
  bool ready();

private:
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  
  std::vector<PlugIn<ValueType>*> _params;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  const unsigned char *_imageIn;
  unsigned char *_imageOut;

  //!Handle on the .so plugin.
  void *_handle;

  //frei0r structs
  f0r_instance_t _instance;
  f0r_plugin_info_t _inverterInfo;

  //!interface function pointers.
  int (*f0r_init)();
  void (*f0r_get_plugin_info)(f0r_plugin_info_t* inverterInfo);
  void (*f0r_get_param_info)(f0r_param_info_t* info, int param_index);
  f0r_instance_t (*f0r_construct)(unsigned int width, unsigned int height);
  void (*f0r_destruct)(f0r_instance_t instance);
  void (*f0r_set_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_get_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_update)(f0r_instance_t instance, double time, const uint32_t* inframe, uint32_t* outframe);

  int _sizeX, _sizeY;

};

#endif
