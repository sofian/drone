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

#include "Control.h"
#include "SignalType.h"
#include "GearControl.h"

enum eButtonState {ON, OFF};

class Gear_PushButton : public GearControl  
{
public:
  static const float  DEFAULT_VALUE;

  static const QString SETTING_OFFVALUE;
  static const QString SETTING_ONVALUE;
  static const QString SETTING_ONESHOT;

  static const QString SETTING_ACCEPTMIDI;
  static const QString SETTING_MIDICHANNEL;
  static const QString SETTING_MIDINOTE;

  Gear_PushButton();
  virtual ~Gear_PushButton();

  void runVideo();
    
  void setState(eButtonState state);
  eButtonState getState(){return _state;}
  float getValue(){return _VALUE_OUT->type()->value();}

protected:
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug*);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug*);
  
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