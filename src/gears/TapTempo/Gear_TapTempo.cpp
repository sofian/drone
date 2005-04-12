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

Gear_TapTempo::Gear_TapTempo(Schema *schema, std::string uniqueName) : Gear(schema, "TapTempo", uniqueName),_lastTapTime(-1),_lastTapValue(0),_scheduledTapTimeStamp(-1),_scheduledTapTimeStampNext(-1),_estimatedTempo(-1),_framesUp(4)
{

  addPlug(_TAP_IN = new PlugIn<ValueType>(this, "Tap", new ValueType(0.0f)));
  addPlug(_RESET= new PlugIn<ValueType>(this, "Reset", new ValueType(0.0f)));
  addPlug(_OFFSET= new PlugIn<ValueType>(this, "Offset", new ValueType(0.0f,-10.0f,10.0f)));

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out"));
}

Gear_TapTempo::~Gear_TapTempo()
{

}

bool Gear_TapTempo::ready()
{
  return(_VALUE_OUT->connected());
}

void Gear_TapTempo::runVideo()
{ 
  // we have a tap
  if(_TAP_IN->type()->value() && _lastTapValue==0)
  {
    if(_lastTapTime!=-1 && (Engine::currentRealTime() - _lastTapTime) > .3)
      _tapSpacings.push_back(Engine::currentRealTime() - _lastTapTime);
    _lastTapTime=Engine::currentRealTime();
    if(_tapSpacings.size()>10)
      _tapSpacings.pop_front();
  }
  
  float lagOffset = -.23;
//  cerr<<setprecision(20)<<setw(20)<<(Engine::currentRealTime())<<endl;
  float divisor;

  if(_tapSpacings.size()>4)
  {
    if(_estimatedTempo==-1)
    {
      float shortest = 10E10;
      for(std::deque<float>::iterator it = _tapSpacings.begin(); it!=_tapSpacings.end();++it)
      {
        if((*it) < shortest)
          shortest=*it;
      }
      divisor=shortest;
    }
    else
      divisor = _estimatedTempo;
   
  
    float avg =0;
    float avgcnt=0;
    for(std::deque<float>::iterator it = _tapSpacings.begin(); it!=_tapSpacings.end();++it)
    {
      float nbeatsInTapLength = MAX(1.0,round(*it/divisor));
      std::cerr<< *it << " "<<nbeatsInTapLength <<"* (" << *it / nbeatsInTapLength << ")" << std::endl;
      avg += *it / nbeatsInTapLength * MIN(nbeatsInTapLength,10.0f);
      avgcnt += MIN(nbeatsInTapLength,10.0f);
    }


    _estimatedTempo = avg/avgcnt;
    std::cerr<<"AVG:"<<_estimatedTempo<<std::endl;

    if(_scheduledTapTimeStamp == -1)
    {
      _scheduledTapTimeStamp = Engine::currentRealTime() + _estimatedTempo;
    }

    // if(_framesUp)
//     {
//       --_framesUp;
//       _VALUE_OUT->type()->setValue(1);
//     }
     if(_TAP_IN->type()->value() && _lastTapValue==0)
    {
      _framesUp = 4;
      //_VALUE_OUT->type()->setValue(1);
      _scheduledTapTimeStampNext = Engine::currentRealTime() + _estimatedTempo - lagOffset;
    }
    else 
    {
      
      if(Engine::currentRealTime() >= _scheduledTapTimeStamp)
      {
        _framesUp = 4;
        _VALUE_OUT->type()->setValue(1);
        if(_scheduledTapTimeStampNext != -1)
        {
          _scheduledTapTimeStamp = _scheduledTapTimeStampNext;
          _scheduledTapTimeStampNext =-1;
        }
        else
          _scheduledTapTimeStamp += _estimatedTempo;
      }
      else
        _VALUE_OUT->type()->setValue(0);
      _scheduledTapTimeStampNext =-1;
      
    }
  }
  _lastTapValue = _TAP_IN->type()->value();
}
