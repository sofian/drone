/* Gear_Frei0r.cpp
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

#include "Gear_Frei0r.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>
#include <dlfcn.h>

#include "GearMaker.h"

extern "C" {
std::string globalFrei0rLib;

void setFrei0rLib(std::string myFrei0rLib)
{
  globalFrei0rLib = myFrei0rLib;
}

Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Frei0r(schema, uniqueName, globalFrei0rLib); // dummy
}

GearInfo getGearInfo()
{
  std::cout << "getGearInfo:" << globalFrei0rLib<<std::endl;
  void *handle = dlopen(globalFrei0rLib.c_str(), RTLD_LAZY);
  std::cout << "getGearInfo 1" << std::endl;
  ASSERT_ERROR_MESSAGE(handle, "fail to load plugin");
  std::cout << "getGearInfo 2" << std::endl;

  GearInfo gearInfo;
  f0r_plugin_info_t pluginInfo;
  void (*f0r_get_plugin_info)(f0r_plugin_info_t*);
  *(void**) (&f0r_get_plugin_info) = dlsym(handle, "f0r_get_plugin_info");
  std::cout << "getGearInfo 3" << std::endl;
  (*f0r_get_plugin_info)(&pluginInfo);
  std::cout << "getGearInfo 4" << std::endl;

  char str[1000];
  sprintf(str, "Frei0r::%s", pluginInfo.name);
  gearInfo.name = str;
  gearInfo.classification = GearClassifications::signal().transform().instance();
  gearInfo.data = (char*) malloc(1000*sizeof(char));
  strcpy((char*)gearInfo.data, globalFrei0rLib.c_str());
  std::cout << "data = " << (char*)gearInfo.data << std::endl;
  
  return gearInfo;
}
}

Gear_Frei0r::Gear_Frei0r(Schema *schema, std::string uniqueName, std::string frei0rLib) : 
Gear(schema, frei0rLib, uniqueName),
_handle(0),
_instance(0),
_sizeX(0),
_sizeY(0),
_frei0rLib(frei0rLib)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  std::cout << "Constructing a Gear_Frei0r with lib " << _frei0rLib << std::endl;

  _handle = dlopen(_frei0rLib.c_str(), RTLD_LAZY);
  ASSERT_ERROR_MESSAGE(_handle, "fail to load plugin");

  //Clear any existing error
  //  dlerror();
    
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

  char* error = dlerror();
  std::cerr << (int)error << std::endl;
  ASSERT_ERROR_MESSAGE(!error, "fail to find f0r_init");

  //now call init
  (*f0r_init)();

  //get infos  
  (*f0r_get_plugin_info)(&_pluginInfo);

  std::cout << "Frei0r plugin: " << _pluginInfo.name << std::endl;
  std::cout << "author: " << _pluginInfo.author << std::endl;        /**< The plugin author                                */
  std::cout << "type: " << _pluginInfo.plugin_type << std::endl;     /**< The plugin type (source or filter)               */
  std::cout << "colormodel: " << _pluginInfo.color_model << std::endl;     /**< The color model used                             */
  std::cout << "version: " << _pluginInfo.frei0r_version << "." << _pluginInfo.major_version << "." << _pluginInfo.minor_version << std::endl;  /**< The frei0r major version this plugin is built for*/
  std::cout << "num_params: " << _pluginInfo.num_params << std::endl;      /**< The number of parameters of the plugin           */
  std::cout << "details: " << _pluginInfo.explanation << std::endl;     /**< An optional explanation string (can be 0)    */
  
  for (int i=0; i<_pluginInfo.num_params; ++i)
  {
    f0r_param_info param_info;
    (*f0r_get_param_info)(&param_info, i);
    switch (param_info.type)
    {
    case F0R_PARAM_BOOL:
    case F0R_PARAM_DOUBLE:
    {    
      PlugIn<ValueType> *param = new PlugIn<ValueType>(this, param_info.name, new ValueType(0, 0, 1));
      addPlug(param);
      _params.push_back(param);
    }
      break;
    case F0R_PARAM_COLOR:
    {    
      PlugIn<ValueType> *r = new PlugIn<ValueType>(this, std::string(param_info.name) + " R", new ValueType(0, 0, 1));
      PlugIn<ValueType> *g = new PlugIn<ValueType>(this, std::string(param_info.name) + " G", new ValueType(0, 0, 1));
      PlugIn<ValueType> *b = new PlugIn<ValueType>(this, std::string(param_info.name) + " B", new ValueType(0, 0, 1));
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
      PlugIn<ValueType> *x = new PlugIn<ValueType>(this, std::string(param_info.name) + " X", new ValueType(0, 0, 1));
      PlugIn<ValueType> *y = new PlugIn<ValueType>(this, std::string(param_info.name) + " Y", new ValueType(0, 0, 1));
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

Gear_Frei0r::~Gear_Frei0r()
{
  (*f0r_destruct)(_instance);
  dlclose(_handle);
}

void Gear_Frei0r::init()
{

}
 
bool Gear_Frei0r::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Frei0r::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  if (_sizeX != (int)_image->width() || _sizeY != (int)_image->height())
  {
    // Rebuild the instance since dimensions have changed.
    if (_instance)      
      (*f0r_destruct)(_instance);
    _sizeX = _image->width();
    _sizeY = _image->height();

    _instance = (*f0r_construct)(_sizeX, _sizeY);
  }

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

  (*f0r_update)(_instance, 0.0, (uint32_t*)_image->data(), (uint32_t*)_outImage->data());
}

