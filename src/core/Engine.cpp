/* Engine.cpp
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

#include "Engine.h"
#include "Gear.h"
#include "Plug.h"
#include "GearMaker.h"
#include <iostream>
#include <unistd.h>


SignalInfo Engine::_signalInfo;
VideoInfo Engine::_videoInfo;
Time_T Engine::_currentTime=0.0;

Engine::Engine(int hwnd) : 
  _hWnd(hwnd),
  _averageLoad(0.0f),
  _playing(false),
  _graphSynched(false)

{
}

Engine::~Engine()
{
}

Time_T Engine::currentTime()
{
  return _currentTime;
}

void Engine::startPlaying()
{
  _playing=true;
  pthread_create(&_playThreadHandle, NULL, playThread, this);
}

void Engine::stopPlaying()
{                        
  if (!_playing)
    return;

  _playing=false;

  //wait for the playThread to finish
  pthread_join(_playThreadHandle, NULL);

}

void *Engine::playThread(void *parent)
{
  Engine *engine = (Engine*) parent;

  engine->_currentTime=0;

  float block_starttime, block_endtime, block_targettime;

  float block_time;

  float real_starttime;
  float real_time=0.0f;
  float sleeptime=0.0f;
  long blockIt=1;
  block_targettime = _signalInfo.blockSize() * _signalInfo.timePerSample() * 1000.0f;

  real_starttime = Timing::time();

  float cumul_load = 0.0f;
  engine->_averageLoad=0.0f;

  for (std::list<Gear*>::iterator it=engine->_gears.begin();it!=engine->_gears.end();++it)
    (*it)->internalPrePlay();

  int currentFrame = 0;

#ifndef SINGLE_THREADED_PLAYBACK
  while (engine->_playing)
  {
#endif    
    block_starttime = Timing::time();
   
    engine->_mainSchema.lock();
    
    engine->_orderedGears = engine->_mainSchema.getDeepOrderedReadyGears();

    //process audio
    for (std::list<Gear*>::iterator it=engine->_orderedGears.begin();it!=engine->_orderedGears.end();++it)
      (*it)->runAudio();

    //process video
    if (real_time >= (currentFrame*_videoInfo.timePerFrame()*1000.0f))
    {
      for (std::list<Gear*>::iterator it=engine->_orderedGears.begin();it!=engine->_orderedGears.end();++it)
      {
        (*it)->runVideo();
      }

      currentFrame++;
    }

    //nsamples-=_signalInfo.blockSize();
    //the currentTime of the engine
    engine->_currentTime += (engine->_signalInfo.blockSize() * engine->_signalInfo.timePerSample());


    block_endtime = Timing::time();
    //the real time that this loop have taken
    real_time = block_endtime - real_starttime;

    //compute AverageLoad
    block_time = block_endtime - block_starttime;
    cumul_load += (block_time /  block_targettime) * 100.0f;
    engine->_averageLoad =  cumul_load / blockIt;

    //temp
    if (!(blockIt%100))
      std::cout << "AverageLoad " << engine->_averageLoad << "%" << std::endl;
    //

    //to maintain real-time processing we have to sleep
    //for the difference between the time we are supposed to be at (_currentTime, the engine time)
    //and the time we are really at (real_time)
    sleeptime = ( (engine->_currentTime*1000.0f) - real_time);

    if (sleeptime > 0)
      Timing::sleep((int)sleeptime);

    blockIt++;

    engine->_mainSchema.unlock();
#ifndef SINGLE_THREADED_PLAYBACK  
  }
#endif               

  
  for (std::list<Gear*>::iterator it=engine->_gears.begin();it!=engine->_gears.end();++it)
    (*it)->internalPostPlay();


  return NULL;
}

void Engine::scheduleGearUpdateSettings(Gear *gear)
{
  _scheduledsGearUpdateSettings.push_back(gear);

  if (!_playing)
    performScheduledGearUpdateSettings();
}

void Engine::performScheduledGearUpdateSettings()
{
  for (std::vector<Gear*>::iterator it=_scheduledsGearUpdateSettings.begin(); it!=_scheduledsGearUpdateSettings.end(); ++it)  
    (*it)->updateSettings();
      
  _scheduledsGearUpdateSettings.clear();
}

