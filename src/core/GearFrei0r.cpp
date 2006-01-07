/* GearFrei0r.cpp
 * Copyright (C) 2004 Mathieu Guindon, Jean-Sebastien Senecal
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

#include "GearFrei0r.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>
#include <dlfcn.h>

#include "GearMaker.h"

GearFrei0r::GearFrei0r(Schema *schema, QString uniqueName, void* handle) : 
  Gear(schema, "", uniqueName),
	_handle(handle),
  _instance(0),
  _sizeX(0),
  _sizeY(0)
{

  //get interface function pointers
  *(void**) (&f0r_init) = dlsym(_handle, "f0r_init");
  *(void**) (&f0r_get_plugin_info) = dlsym(_handle, "f0r_get_plugin_info");
  *(void**) (&f0r_get_param_info) = dlsym(_handle, "f0r_get_param_info");
  *(void**) (&f0r_construct) = dlsym(_handle, "f0r_construct");
  *(void**) (&f0r_destruct) = dlsym(_handle, "f0r_destruct");
  *(void**) (&f0r_set_param_value) = dlsym(_handle, "f0r_set_param_value");
  *(void**) (&f0r_get_param_value) = dlsym(_handle, "f0r_get_param_value");
  *(void**) (&f0r_update) = dlsym(_handle, "f0r_update");
  //...

  const char* error = dlerror();
  std::cerr << (int)error << std::endl;
  ASSERT_ERROR_MESSAGE(!error, "fail to find f0r_init");

  //now call init
  (*f0r_init)();

  //get infos  
  (*f0r_get_plugin_info)(&_pluginInfo);

  //set type
  char str[1000];
  sprintf(str, "f0r::%s", _pluginInfo.name);
  _Type = str;

  // Set all plugs.

  if (_pluginInfo.plugin_type == F0R_PLUGIN_TYPE_FILTER)
    addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  else
  {
    addPlug(_VIDEO_X_IN = new PlugIn<ValueType>(this, "Width", true, new ValueType(320, 0, 640)));
    addPlug(_VIDEO_Y_IN = new PlugIn<ValueType>(this, "Height", true, new ValueType(240, 0, 480)));
  }

  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
    
  for (int i=0; i<_pluginInfo.num_params; ++i)
  {
    f0r_param_info param_info;
    (*f0r_get_param_info)(&param_info, i);
    switch (param_info.type)
    {
    case F0R_PARAM_BOOL:
    {    
      PlugIn<ValueType> *param = new PlugIn<ValueType>(this, param_info.name, false, new ValueType(0, 0, 1));
      addPlug(param);
      _params.push_back(param);
    }
    break;
    case F0R_PARAM_DOUBLE:
    {    
      PlugIn<ValueType> *param = new PlugIn<ValueType>(this, param_info.name, false, new ValueType(0.5, 0, 1));
      addPlug(param);
      _params.push_back(param);
    }
      break;
    case F0R_PARAM_COLOR:
    {    
      PlugIn<ValueType> *b = new PlugIn<ValueType>(this, QString(param_info.name) + " B", false, new ValueType(0.5, 0, 1));
      PlugIn<ValueType> *g = new PlugIn<ValueType>(this, QString(param_info.name) + " G", false, new ValueType(0.5, 0, 1));
      PlugIn<ValueType> *r = new PlugIn<ValueType>(this, QString(param_info.name) + " R", false, new ValueType(0.5, 0, 1));
      addPlug(r);
      addPlug(g);
      addPlug(b);
      _params.push_back(r);
      _params.push_back(g);
      _params.push_back(b);
    }
      break;
    case F0R_PARAM_POSITION:
    {    
      PlugIn<ValueType> *x = new PlugIn<ValueType>(this, QString(param_info.name) + " X", false, new ValueType(0.5, 0, 1));
      PlugIn<ValueType> *y = new PlugIn<ValueType>(this, QString(param_info.name) + " Y", false, new ValueType(0.5, 0, 1));
      addPlug(x);
      addPlug(y);
      _params.push_back(x);
      _params.push_back(y);
    }
      break;
    default:
      std::cerr << "Wrong parameter type " << param_info.type << std::endl;
    }
  }
  std::cout << "init done" << std::endl; 
}

GearFrei0r::~GearFrei0r()
{
  (*f0r_destruct)(_instance);
}

void GearFrei0r::runVideo()
{
  if (_pluginInfo.plugin_type == F0R_PLUGIN_TYPE_FILTER)
  {
    _image = _VIDEO_IN->type();
    if (_image->isNull())
      return;

    if (_sizeX != (int)_image->width() || _sizeY != (int)_image->height())
    {
      // Rebuild the instance since dimensions have changed.
      if (_instance)
        (*f0r_destruct)(_instance);
      _sizeX = _image->width();
      _sizeY = _image->height();
      
      _instance = (*f0r_construct)(_sizeX, _sizeY);
    }
    _imageIn = (uint32_t*)_image->data();
  }
  else
  {
    int width = (int)_VIDEO_X_IN->type()->value();
    int height = (int)_VIDEO_Y_IN->type()->value();

    if (_sizeX != width || _sizeY != height)
    {
      // Rebuild the instance since dimensions have changed.
      if (_instance)
        (*f0r_destruct)(_instance);
      _sizeX = width;
      _sizeY = height;
      
      _instance = (*f0r_construct)(_sizeX, _sizeY);
    }
    _imageIn = 0;
  }
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_sizeX, _sizeY);

  for (int i=0,k=0; i<_pluginInfo.num_params; ++i)
  {
    f0r_param_info param_info;
    (*f0r_get_param_info)(&param_info, i);
    switch (param_info.type)
    {
    case F0R_PARAM_BOOL:
    {      
      f0r_param_bool param_bool = _params[k++]->type()->value();
      (*f0r_set_param_value)(_instance, (void*)&param_bool, i);
    }
      break;
    case F0R_PARAM_DOUBLE:
    {      
      f0r_param_double param_double = _params[k++]->type()->value();
      (*f0r_set_param_value)(_instance, (void*)&param_double, i);
    }
      break;
    case F0R_PARAM_COLOR:
    {      
      f0r_param_color param_color;
      param_color.r =  _params[k++]->type()->value();
      param_color.g =  _params[k++]->type()->value();
      param_color.b =  _params[k++]->type()->value();      
      (*f0r_set_param_value)(_instance, (void*)&param_color, i);
    }
      break;
    case F0R_PARAM_POSITION:
    {    
      f0r_param_position param_position;
      param_position.x =  _params[k++]->type()->value();
      param_position.y =  _params[k++]->type()->value();      
      (*f0r_set_param_value)(_instance, (void*)&param_position, i);
    }
      break;
    default:
      error("Wrong parameter type: %d", param_info.type);
    }
  }

  // note: XXX incomplete XXX we have to switch on BGRA
  (*f0r_update)(_instance, Engine::currentTime(), _imageIn, (uint32_t*)_outImage->data());
}

