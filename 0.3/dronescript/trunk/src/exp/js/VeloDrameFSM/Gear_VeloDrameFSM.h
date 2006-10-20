/* Gear_VeloDrameFSM.h
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

#ifndef GEAR_VELODRAMEFSM_INCLUDED
#define GEAR_VELODRAMEFSM_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "SummedAreaTable.h"

class Gear_VeloDrameFSM : public Gear
{
public:
  enum eAction
    {
      IGNORE,
      LISTEN,
      TALK,
      TALKMORE,
      TALKEVENMORE,
      N_ACTIONS
    };

  enum eState
    {
      SILENCE,
      DIALOG,
      MONO_LEFT,
      MONO_RIGHT,
      FIGHT,
      N_STATES
    };
  
  enum eValence
    {
      STOP,
      LOW,
      MID,
      HIGH,
      N_VALENCES
    };

//   typedef std::pair<eState,eState> State;
//   typedef std::pair< State,
//                      std::pair<eValence,eValence> > TransitionId;
  
//   typedef std::map<TransitionId, State> TransitionMap;
  
  Gear_VeloDrameFSM(Schema *schema, std::string uniqueName);
  virtual ~Gear_VeloDrameFSM();

  void runVideo();

  bool ready();

protected:
  void internalInit();

private:

  PlugOut<ValueType> *_ACTION_L_OUT;
  PlugOut<ValueType> *_ACTION_R_OUT;

  PlugIn<ValueType> *_ACTION_L_IN;
  PlugIn<ValueType> *_ACTION_R_IN;
  PlugIn<ValueType> *_VALUE_L_IN;
  PlugIn<ValueType> *_VALUE_R_IN;
  PlugIn<ValueType> *_MID_THRESHOLD_IN;
  PlugIn<ValueType> *_HIGH_THRESHOLD_IN;

  //local var
  static eState state(eValence leftVal, eValence rightVal); 
  eValence valence(float value) const;
};

#endif
