/* Gear_SVGLoader.cpp
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

enum { flip_y = true };

#include "Gear_SVGLoader.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>
//#include <fstream>

#include "GearMaker.h"

const std::string Gear_SVGLoader::SETTING_FILENAME = "Filename";


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_SVGLoader(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "SVGLoader";
  gearInfo.classification = GearClassifications::video().vectorial().instance();
  return gearInfo;
}
}

Gear_SVGLoader::Gear_SVGLoader(Schema *schema, std::string uniqueName) : Gear(schema, "SVGLoader", uniqueName)
{
  addPlug(_VEC_OUT = new PlugOut<VectorialType>(this, "VecOUT",false));
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    


}

Gear_SVGLoader::~Gear_SVGLoader()
{

}

void Gear_SVGLoader::onUpdateSettings()
{
  agg::svg::parser p(m_path);
 
  std::fstream fin;
  fin.open(_settings.get(SETTING_FILENAME)->valueStr().c_str(),std::ios::in);
  if( !fin.is_open() )
  {  
    std::cerr<<"SVG Loader : Can't open file :"<<_settings.get(SETTING_FILENAME)->valueStr().c_str()<<std::endl;
    return;
  }
  p.parse(_settings.get(SETTING_FILENAME)->valueStr().c_str());
  m_path.arrange_orientations();
  m_path.bounding_rect();

}

void Gear_SVGLoader::runVideo()
{
  _VEC_OUT->type()->setPath(&m_path);
}

