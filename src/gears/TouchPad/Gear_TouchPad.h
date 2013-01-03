/* Gear_TouchPad.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_TOUCHPAD_INCLUDED
#define GEAR_TOUCHPAD_INCLUDED

#include "GearControl.h"
#include "ValueType.h"


class Gear_TouchPad : public GearControl
{
public:
  static const float  DEFAULT_VALUE;

  static const std::string SETTING_HORIZONTAL_LOWERBOUND;
  static const std::string SETTING_HORIZONTAL_HIGHERBOUND;
  static const std::string SETTING_VERTICAL_LOWERBOUND;
  static const std::string SETTING_VERTICAL_HIGHERBOUND;

  Gear_TouchPad(Schema *schema, std::string uniqueName);
  virtual ~Gear_TouchPad();

  void save(QDomDocument &doc, QDomElement &gearElem);
  void load(QDomElement &gearElem);

  void runAudio();

  void setHorizontalValue(float value);
  float getHorizontalValue(){return _VALUE_H_OUT->type()->value();};
  void setVerticalValue(float value);
  float getVerticalValue(){return _VALUE_V_OUT->type()->value();};

protected:
  GearGui* createGearGui(QGraphicsScene *scene);//we want a slider so we overload
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug*);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug*);
  Control* internalCreateControl(ControlPanel *){return NULL;}//TODO finish this

private:
  PlugOut<ValueType>* _VALUE_H_OUT;
  PlugOut<ValueType>* _VALUE_V_OUT;
  
  bool _acceptHorizontalHint;
  bool _acceptVerticalHint;

  void onUpdateHorizontal();
  void onUpdateVertical();
};

#endif 
