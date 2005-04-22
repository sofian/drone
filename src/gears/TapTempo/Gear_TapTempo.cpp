/* Gear_TapTempo.cpp
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

#include "Math.h"
#include "Gear_TapTempo.h"
#include "Engine.h"

#include "GearMaker.h"
#include <iomanip>

using namespace std;

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_TapTempo(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "TapTempo";
  gearInfo.classification = GearClassifications::signal().instance();
  return gearInfo;
}
}

Gear_TapTempo::Gear_TapTempo(Schema *schema, std::string uniqueName) : Gear(schema, "TapTempo", uniqueName)
{

  addPlug(_TAP_IN   = new PlugIn<ValueType>(this, "Tap", new ValueType(0.0f)));
  addPlug(_DECAY_IN = new PlugIn<ValueType>(this, "Decay", new ValueType(0.9f, 0.0f, 1.0f)));

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out"));
}

Gear_TapTempo::~Gear_TapTempo()
{

}

bool Gear_TapTempo::ready()
{
  return(_VALUE_OUT->connected());
}

void Gear_TapTempo::internalInit()
{
  _estimatedTempo = -1;
  _lastTapTime = -1;
  _scheduledBeatTimeStamp = FLT_MAX;
}

void Gear_TapTempo::runVideo()
{
  float currentTime = Engine::currentTime();
  
  // We have a tap.
  if(_TAP_IN->type()->boolValue())
  {
    if (_lastTapTime <= 0)
      // This is the very first tap.
    {
      // Beat now!
      _scheduledBeatTimeStamp = currentTime;

      // Memorize the tap time.
      _lastTapTime = currentTime;
    }
    else
    {
      // The interval between two taps.
      float interval = currentTime - _lastTapTime;

      // Memorize the tap time.
      _lastTapTime = currentTime;

      //    std::cout << "Tap in: " << currentTime << ", " << interval << ", " << _lastTapTime << std::endl;
      if (_estimatedTempo >= 0)
        // There already was an estimated tempo.
      {
        if (interval < (2*_estimatedTempo))
          // Tapped inside limited interval => just readjust tempo.
        {
          //std::cout << "Tap < 2*estimatedTempo";
          float decay = CLAMP( _DECAY_IN->type()->value(), 0.0f, 1.0f );
          float preestimatedTempo = _estimatedTempo;
          // Adjust estimated tempo using moving average.
          _estimatedTempo = (1.0f-decay)*_estimatedTempo + decay*interval; // reestimate tempo w/ moving average
          _scheduledBeatTimeStamp += (_estimatedTempo - preestimatedTempo); // readjust schedule
        }
        else
          // Tapped far away from last sequence => reset tempo.
        {
          //std::cout << "Tap > 2*estimatedTempo : reset";
          // Reset tempo.
          _estimatedTempo = -1;
          // Beat now!
          _scheduledBeatTimeStamp = currentTime;
        }
      }
      else
        // We have no estimated tempo yet.
      {
        //std::cout << "Tap estimatedTempo < 0 : new";
        // Estimate tempo with current interval.
        _estimatedTempo = interval;

        // Beat now!
        _scheduledBeatTimeStamp = currentTime;
      }
    }

  }

  //std::cout << " currentTime = " << currentTime << ", scheduled = " << _scheduledBeatTimeStamp << ", tempo = " << _estimatedTempo;
  if (currentTime >= _scheduledBeatTimeStamp)
    // Beat now!
  {
    //std::cout << " ### Beat it! : " << (_estimatedTempo >= 0 ? "ok":"infinite") << std::endl;
    // Schedule next beat.
    if (_estimatedTempo >= 0)
      _scheduledBeatTimeStamp += _estimatedTempo;
    else
      _scheduledBeatTimeStamp = FLT_MAX;
    // Beat it!
    _VALUE_OUT->type()->setValue(1.0f);
  }
  else
  {
    //std::cout << " ### No beat." << std::endl;
    // No beat.
    _VALUE_OUT->type()->setValue(0.0f);
  }
  
  //std::cout << _estimatedTempo << "," << _scheduledBeatTimeStamp << "," << _lastTapTime << "," << _VALUE_OUT->type()->value() << "," << currentTime() << std::endl;
}
