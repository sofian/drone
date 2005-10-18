/* Gear_TA_DataSource.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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
//inspired from Using libavformat and libavcodec by Martin Böhme (boehme@inb.uni-luebeckREMOVETHIS.de) 

#include <iostream>
#include "Gear_TA_MapInfoLayer.h"
#include "Engine.h"
#include "StringUtils.h"
#include "GearMaker.h"
#include "AggWrapper.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_MapInfoLayer(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_MapInfoLayer";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_MapInfoLayer::Gear_TA_MapInfoLayer(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_MapInfoLayer", uniqueName)
{

  addPlug(_DATA_IN = new   PlugIn<TA_DataType>(this,"DATA",true)); 
  addPlug(_VEC_IN = new   PlugIn<VectorialType>(this,"VecIn",true)); 
  addPlug(_RAX = new   PlugIn<ValueType>(this,"RAX",true));
  addPlug(_RAY = new   PlugIn<ValueType>(this,"RAY",true));
  
  addPlug(_XOFF = new   PlugIn<ValueType>(this,"XOff",true));
  addPlug(_YOFF = new   PlugIn<ValueType>(this,"YOff",true));
  addPlug(_SCALE = new   PlugIn<ValueType>(this,"Scale",true));
  
  addPlug(_RADIUS = new   PlugIn<ValueType>(this,"Radius",true));
  addPlug(_MAGN = new   PlugIn<ValueType>(this,"Magn",true));
  
  addPlug(_VIDEO_IN = new   PlugIn<VideoRGBAType>(this,"ImgIn",true));
  addPlug(_VIDEO_OUT = new   PlugOut<VideoRGBAType>(this,"ImgOut",true));
  //addPlug(_VEC_OUT = new   PlugOut<VectorialType>(this,"VecOut",)); 
  

}

Gear_TA_MapInfoLayer::~Gear_TA_MapInfoLayer()
{
}

void Gear_TA_MapInfoLayer::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;
  TA_DataType* graph = _DATA_IN->type();

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _data = (unsigned int*)_image->data();    
  _outData = (unsigned int*)_outImage->data();
  
  memcpy(&_outData[0], &_data[0], _image->width() * _image->height() * 4);

  AggWrapper aggw(_outImage);
  aggw.m_globtrans = (_VEC_IN->type()->path()->m_global_trans);
  double rax = _RAX->type()->value();
  double ray = _RAY->type()->value();
  double scale = _SCALE->type()->value();
  double xoff = _XOFF->type()->value();
  double yoff = _YOFF->type()->value();
 
  std::vector<disto_t> pt;
disto_t di;
di.x=rax;
di.y=ray;
di.rad=_RADIUS->type()->value();;
di.magn=_MAGN->type()->value();;
  pt.push_back(di);
  _VEC_IN->type()->path()->setDisto(pt);
  
  for(TA_DataType::const_iterator it= graph->begin(); it!=graph->end(); ++it)
	{
		TA_CityVertex ver = it->second;
		aggw.circle( ver.x , ver.y, 10 * (float)(rand()%10) / 10, agg::rgba8(200,100,30,125));
	}
  
  aggw.circle( rax,ray, 10, agg::rgba8(00,100,230,125));
}
