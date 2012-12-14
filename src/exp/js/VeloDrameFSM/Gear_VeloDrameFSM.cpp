/* Gear_VeloDrameFSM.cpp
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

#include "Gear_VeloDrameFSM.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

#include "DroneMath.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VeloDrameFSM(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VeloDrameFSM";
  gearInfo.classification = GearClassifications::unclassified().instance();
  return gearInfo;
}
}

Gear_VeloDrameFSM::Gear_VeloDrameFSM(Schema *schema, std::string uniqueName) : Gear(schema, "VeloDrameFSM", uniqueName)
{
  addPlug(_ACTION_L_OUT = new PlugOut<ValueType>(this, "ActionLOut", new ValueType(0, 0, N_ACTIONS)));
  addPlug(_ACTION_R_OUT = new PlugOut<ValueType>(this, "ActionROut", new ValueType(0, 0, N_ACTIONS)));

  addPlug(_VALUE_L_IN = new PlugIn<ValueType>(this, "ValueLIn", new ValueType(0, 0, 1)));
  addPlug(_VALUE_R_IN = new PlugIn<ValueType>(this, "ValueRIn", new ValueType(0, 0, 1)));
  addPlug(_ACTION_L_IN = new PlugIn<ValueType>(this, "ActionLIn", new ValueType(0, 0, N_ACTIONS)));
  addPlug(_ACTION_R_IN = new PlugIn<ValueType>(this, "ActionRIn", new ValueType(0, 0, N_ACTIONS)));
  addPlug(_MID_THRESHOLD_IN = new PlugIn<ValueType>(this, "MidThresh", new ValueType()));
  addPlug(_HIGH_THRESHOLD_IN = new PlugIn<ValueType>(this, "HighThresh", new ValueType()));
}

Gear_VeloDrameFSM::~Gear_VeloDrameFSM()
{
}

bool Gear_VeloDrameFSM::ready()
{
  return true;
}

void Gear_VeloDrameFSM::internalInit()
{
}

void Gear_VeloDrameFSM::runVideo()
{ 
  eAction leftAction  = (eAction)_ACTION_L_IN->type()->intValue();
  eAction rightAction = (eAction)_ACTION_R_IN->type()->intValue();
  eValence leftVal  = (eValence)valence(_VALUE_L_IN->type()->value());
  eValence rightVal = (eValence)valence(_VALUE_R_IN->type()->value());
  eAction leftNextAction, rightNextAction;

  eState currentState = state(leftVal, rightVal);
  
  switch (currentState)
  {
  case SILENCE:
    leftNextAction = rightNextAction = IGNORE;
    break;
  case MONO_LEFT:
    leftNextAction = (eAction) MIN( TALK + (leftVal - LOW), (int)TALKEVENMORE );
    if (rightVal == STOP)
      rightNextAction = IGNORE;
    else
      rightNextAction = LISTEN;
    break;
  case MONO_RIGHT:
    rightNextAction = (eAction) MIN( TALK + (rightVal - LOW), (int)TALKEVENMORE );
    if (leftVal == STOP)
      leftNextAction = IGNORE;
    else
      leftNextAction = LISTEN;
    break;
  case DIALOG:
    if (leftAction >= TALK) // la personne de gauche vient de parler
    {
      leftNextAction  = LISTEN;
      rightNextAction = (eAction) MIN( TALK + (rightVal - LOW), (int)TALKEVENMORE );
    }
    else
    {
      leftNextAction  = (eAction) MIN( TALK + (leftVal - LOW), (int)TALKEVENMORE );
      rightNextAction = LISTEN;
    }
    break;
  case FIGHT:
  default:
    leftNextAction = rightNextAction = TALKEVENMORE;
    break;
  }
  
  _ACTION_L_OUT->type()->setValue((float)leftNextAction);
  _ACTION_R_OUT->type()->setValue((float)rightNextAction);
}

Gear_VeloDrameFSM::eState Gear_VeloDrameFSM::state(Gear_VeloDrameFSM::eValence leftVal, Gear_VeloDrameFSM::eValence rightVal)
{
  if (leftVal == STOP)
  {
    if (rightVal == STOP)
      return SILENCE;
    else
      return MONO_RIGHT;
  }
  else
  {
    if (rightVal == STOP)
      return MONO_LEFT;
    else // les deux becannes roulent!
    {
      if (leftVal == rightVal)
      {
        if (leftVal == HIGH)
          return FIGHT;
        else
          return DIALOG;
      }
      else // elles roulent pas a la meme vitesse...
      {
        if (abs(leftVal - rightVal) == 1)
          return DIALOG;
        else if (leftVal > rightVal)
          return MONO_LEFT;
        else
          return MONO_RIGHT;
      }
    }
  }
}

Gear_VeloDrameFSM::eValence Gear_VeloDrameFSM::valence(float value) const
{
  if (value <= 0)
    return STOP;
  else  if (value < _MID_THRESHOLD_IN->type()->value())
    return LOW;
  else if (value < _HIGH_THRESHOLD_IN->type()->value())
    return MID;
  else
    return HIGH;
}
