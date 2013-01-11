/* Gear_AudioPushButton.h
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

#ifndef GEAR_AUDIOPUSHBUTTON_INCLUDED
#define GEAR_AUDIOPUSHBUTTON_INCLUDED

#include "Gear.h"
#include "SignalType.h"

enum eButtonState {ON, OFF};

class Gear_AudioPushButton : public Gear  
{
public:
  static const float  DEFAULT_VALUE;

  static const QString SETTING_OFFVALUE;
  static const QString SETTING_ONVALUE;
  static const QString SETTING_ONESHOT;

  Gear_AudioPushButton(Schema *schema, QString uniqueName);
  virtual ~Gear_AudioPushButton();

  void runAudio();
  
  void setState(eButtonState state);
  eButtonState getState(){return _state;}
  float getValue(){return _VALUE_OUT->type()->value();}

protected:
  GearGui* createGearGui(QGraphicsScene *scene);//we want a AudioPushButton so we overload
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
