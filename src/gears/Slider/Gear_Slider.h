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

#include "GearControl.h"
#include "ValueType.h"

class ControlSlider;

class Gear_Slider : public GearControl  
{
public:
  static const float  DEFAULT_VALUE;

  static const QString SETTING_LOWERBOUND;
  static const QString SETTING_HIGHERBOUND;

  static const QString SETTING_ACCEPTMIDI;
  static const QString SETTING_MIDICHANNEL;
  static const QString SETTING_MIDICONTROLLER;

  Gear_Slider();
  virtual ~Gear_Slider();

  void runAudio();

  void setValue(float  value);
  float getValue(){return _VALUE_OUT->type()->value();};

protected:

  void internalSave(QDomDocument &doc, QDomElement &gearElem);
  void internalLoad(QDomElement &gearElem);

  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug*);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug*);

private:
  PlugOut<ValueType>* _VALUE_OUT;  
  
  bool _acceptHint;
};

#endif 
