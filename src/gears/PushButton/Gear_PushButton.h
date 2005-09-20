/* Gear_PushButton.h
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

#ifndef GEAR_PUSHBUTTON_INCLUDED
#define GEAR_PUSHBUTTON_INCLUDED

#include "GearControl.h"
#include "SignalType.h"

enum eButtonState {ON, OFF};

class Gear_PushButton : public GearControl  
{
public:
  static const float  DEFAULT_VALUE;

  static const std::string SETTING_OFFVALUE;
  static const std::string SETTING_ONVALUE;
  static const std::string SETTING_ONESHOT;

  static const std::string SETTING_ACCEPTMIDI;
  static const std::string SETTING_MIDICHANNEL;
  static const std::string SETTING_MIDINOTE;

  Gear_PushButton(Schema *schema, std::string uniqueName);
  virtual ~Gear_PushButton();

  void runVideo();
  
  void setState(eButtonState state);
  eButtonState getState(){return _state;}
  float getValue(){return _VALUE_OUT->type()->value();}

  bool ready();

protected:
  GearGui* createGearGui(QCanvas *canvas);//we want a PushButton so we overload
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug*);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug*);
  
  Control* internalCreateControl(ControlPanel *){return NULL;}//TODO: finish this
  
private:
  PlugOut<ValueType>* _VALUE_OUT;  
  
  bool _acceptHint;
  bool _lastWasOn;
  eButtonState _state;
  // used to protected a very short click from being lost by a mouse release before it is seen by runVideo
  bool _haveUnconsumedClick;
  bool _havePendingRelease;

};

#endif 
