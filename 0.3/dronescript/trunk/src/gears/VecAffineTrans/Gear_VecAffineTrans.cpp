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

#include "Gear_VecAffineTrans.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>
#include <fstream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VecAffineTrans(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VecAffineTrans";
  gearInfo.classification = GearClassifications::video().vectorial().instance();
  return gearInfo;
}
}

Gear_VecAffineTrans::Gear_VecAffineTrans(Schema *schema, std::string uniqueName) : Gear(schema, "VecAffineTrans", uniqueName),
                                                                         m_min_x(0.0),
                                                                         m_min_y(0.0),
                                                                         m_max_x(0.0),
                                                                         m_max_y(0.0),
                                                                         m_x(0.0),
                                                                         m_y(0.0)
{
  addPlug(_VEC_OUT = new PlugOut<VectorialType>(this, "VecOUT",true)); 
  addPlug(_VEC_IN = new PlugIn<VectorialType>(this, "VecIn",true));
  addPlug(_X_IN = new PlugIn<ValueType>(this, "x", false, new ValueType(0.0f, -10.0f, 10.0f)));
  addPlug(_Y_IN = new PlugIn<ValueType>(this, "y", false, new ValueType(0.0f, -10.0f, 10.0f)));
  addPlug(_SCALE_IN = new PlugIn<ValueType>(this, "scale", false, new ValueType(1.0f, 0.1f, 2.0f)));
  addPlug(_ROTATE_IN = new PlugIn<ValueType>(this, "rotat", false, new ValueType(0.0f, 0.0f, 10.0f)));
}

Gear_VecAffineTrans::~Gear_VecAffineTrans()
{

}

void Gear_VecAffineTrans::runVideo()
{

  float x = _X_IN->type()->value();
  float y = _Y_IN->type()->value();
  float scale = _SCALE_IN->type()->value();
  float rotate = _ROTATE_IN->type()->value();

  m_path =  (_VEC_IN->type()->path());  
  
  agg::trans_affine mtx;
//std::cerr<<"!!!!!!!!!!"<<m_path->getMinX()<<","<<m_path->getMaxX()<<"   "<<m_path->getMinY()<<","<<m_path->getMaxY()<<std::endl;

  //mtx *= agg::trans_affine_translation((m_path->getMinX() + m_path->getMaxX()) * -0.5, (m_path->getMinY() + m_path->getMaxY()) * -0.5);
  mtx *= agg::trans_affine_translation(-x, -y);

  mtx *= agg::trans_affine_scaling(scale);
  mtx *= agg::trans_affine_rotation(rotate);
  //mtx *= agg::trans_affine_translation((m_path->getMinX() + m_path->getMaxX()) * 0.5 * scale + x*scale, (m_path->getMinY() + m_path->getMaxY()) * 0.5 * scale + y*scale);
//mtx *= agg::trans_affine_translation(x*scale, y*scale);
  
  m_path->setGlobalTransform(mtx);	
  _VEC_OUT->type()->setPath(m_path);

}

