/* Gear_SimpleDelay.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

// Gear_SimpleDelay.cpp: implementation of the Gear_SimpleDelay class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_SimpleDelay.h"

#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SimpleDelay, Gear_SimpleDelay, "SimpleDelay")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_SimpleDelay::Gear_SimpleDelay(Engine *engine, std::string name) : Gear(engine, "SimpleDelay", name)
{
  addPlug(_PARAM_FEEDBACK = new PlugIn<SignalType>(this, "Feedback", new SignalType(0.3f)));
  addPlug(_PARAM_TIME = new PlugIn<SignalType>(this, "Time", new SignalType(0.3f)));
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "Input", new SignalType(0.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Output"));  
  _SmoothTime=false;
}

Gear_SimpleDelay::~Gear_SimpleDelay()
{

}

bool Gear_SimpleDelay::ready()
{
  return(_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

float _HARDCLIP(float X, float MIN, float MAX) 
{
  if (X<MIN)
    return MIN;
  if (X>MAX)
    return MAX;
  return X;
}

void Gear_SimpleDelay::runAudio()
{    
/*     SignalType bufferfeed  = _PARAM_FEEDBACK->type();                                                                      */
/*     SignalType buffertime  = _PARAM_TIME->type();                                                                          */
/*     SignalType bufferin = _AUDIO_IN->type();                                                                               */
/*     SignalType bufferout = _AUDIO_OUT->type();                                                                             */
/*                                                                                                                             */
/*     static double t=0;                                                                                                      */
/*     int signal_blocksize = _Engine->audioBlockSize();                                                                       */
/*                                                                                                                             */
/*     if(!_SmoothTime)                                                                                                        */
/*     {                                                                                                                       */
/*         int ndefaults;                                                                                                      */
/*         Signal_T defaultval;                                                                                                */
/*         SignalType  block1,*block2;                                                                                          */
/*         int block1len,block2len;                                                                                            */
/*                                                                                                                             */
/*         // use first value of buffertime as delay time for whole block processing (could use average)                       */
/*         int past_samples = _HARDCLIP(_PARAM_TIME->secondsToElements(*buffertime), 0, _PARAM_TIME->secondsToElements(2.0f)); */
/*                                                                                                                             */
/*         //_AUDIO_OUT->getPastTimeBlockInfo(-past_samples+1, signal_blocksize, ndefaults, defaultval,                        */
/*         //                              block1,block2,block1len,block2len);                                                 */
/*         int blockpos=0;                                                                                                     */
/*         while(ndefaults--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + defaultval;                                                        */
/*         while(block1len--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + *(block1++);                                                       */
/*         while(block2len--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + *(block2++);                                                       */
/*                                                                                                                             */
/*         if(blockpos!=signal_blocksize)                                                                                      */
/*             _TRACE("Bug here 123123 "<<endl);                                                                               */
/*                                                                                                                             */
/*     }                                                                                                                       */
/*     else                                                                                                                    */
/*     {                                                                                                                       */
/*         for(int i=0; i<signal_blocksize; i++)                                                                               */
/*             bufferout[i] = bufferin[i]                                                                                      */
/*                     + *_AUDIO_OUT->getPastTimeValue(_PARAM_TIME->secondsToElements(buffertime[i]));                         */
/*     }                                                                                                                       */

}
