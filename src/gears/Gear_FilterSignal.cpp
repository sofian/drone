/* Gear_FilterSignal.cpp
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

// Gear_FilterSignal.cpp: implementation of the Gear_FilterSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_FilterSignal.h"
#include "Engine.h"

#include "GearMaker.h"

#include <math.h>

Register_Gear(MAKERGear_FilterSignal, Gear_FilterSignal, "FilterSignal")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_FilterSignal::Gear_FilterSignal(Engine *engine, std::string name) : Gear(engine, "FilterSignal", name),
                                                                         _convolutionShape()
{    
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", new SignalType(0.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));

  int blockSize = Engine::signalInfo().blockSize();
  _convolutionShape.resize(blockSize);

  float f=440.0f;
  float wc=2.0 * 6.28318f * f * blockSize/44100.0f;

  for (int i=0;i<blockSize;i++)
    _convolutionShape[i] = sin(wc*((float)i - ((float)blockSize/2.0f))) / (3.14f * ((float)i - ((float)blockSize/2.0f)));

}

Gear_FilterSignal::~Gear_FilterSignal()
{

}

bool Gear_FilterSignal::ready()
{
  return(_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_FilterSignal::init()
{    
  if (_freqAnalyzer!=NULL)
    delete _freqAnalyzer;

  _freqAnalyzer = new FrequencyAnalyser(Engine::signalInfo().blockSize());    
}


void Gear_FilterSignal::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();
  float *bufferout = _AUDIO_OUT->type()->data();

  float kernel[Engine::signalInfo().blockSize()];

  for (int i=0;i<Engine::signalInfo().blockSize()/2;++i)
  {
    kernel[i]=1.0f;
  }

  for (int i=Engine::signalInfo().blockSize()/2;i<Engine::signalInfo().blockSize();++i)
  {
    kernel[i]=0.0f;
  }


  //_freqAnalyzer->performAnalysis(bufferin);
  //_freqAnalyzer->backward(bufferout);

  _freqAnalyzer->filter(bufferin, bufferout);
  //_freqAnalyzer->frequencyToTime(kernel, bufferout);

}
