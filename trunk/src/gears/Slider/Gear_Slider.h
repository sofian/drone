/* Gear_Slider.h
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

#ifndef GEAR_SLIDER_INCLUDED
#define GEAR_SLIDER_INCLUDED

#include "Gear.h"
#include "ValueType.h"

class ControlSlider;

class Gear_Slider : public Gear  
{
public:
  static const float  DEFAULT_VALUE;

  static const std::string SETTING_LOWERBOUND;
  static const std::string SETTING_HIGHERBOUND;

  static const std::string SETTING_ACCEPTMIDI;
  static const std::string SETTING_MIDICHANNEL;
  static const std::string SETTING_MIDICONTROLLER;

  Gear_Slider(Schema *schema, std::string uniqueName);
  virtual ~Gear_Slider();

  void save(QDomDocument &doc, QDomElement &gearElem);
  void load(QDomElement &gearElem);

  void runAudio();

  void setValue(float  value);
  float getValue(){return _VALUE_OUT->type()->value();};

  bool ready();

  

protected:
  GearGui* createGearGui(QCanvas *canvas);//we want a slider so we overload
  Control* createCustomControl(ControlPanel *parent);
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug*);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug*);

private:
  PlugOut<ValueType>* _VALUE_OUT;  
  
  bool _acceptHint;
};

#endif 
