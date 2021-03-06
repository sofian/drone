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
 #include "MidiEngine.h"
#include "Gear.h"
#include "Plug.h"
#include "GearMaker.h"
#include "MetaGear.h"
#include <iostream>
#include <unistd.h>


SignalInfo Engine::_signalInfo;
VideoInfo Engine::_videoInfo;
Time_T Engine::_currentTime=0.0;

Engine::Engine(int hwnd) :
  _hWnd(hwnd),
  _averageLoad(0.0f),
  _graphSynched(false),
  _playing(false)
{  
  _mainMetaGear = new MetaGear(NULL, "Main", "Main");  
  _mainMetaGear->getInternalSchema()->addSchemaEventListener(this);
}

Engine::~Engine()
{
}

Time_T Engine::currentTime()
{
  return _currentTime;
}

float Engine::currentRealTime()
{
  timeval t;
  //timezone tz;
  
  gettimeofday(&t,NULL);
  
  return -1.112e09 +t.tv_sec + (float)t.tv_usec/1000000;
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
  Schema *mainSchema = engine->_mainMetaGear->getInternalSchema();

  engine->_currentTime=0;

  Time_T block_starttime, block_endtime, block_targettime;

  Time_T block_time;

  Time_T real_starttime;
  Time_T real_time=0.0;
  Time_T sleeptime=0.0f;

  long blockIt=1;
  block_targettime = _signalInfo.blockSize() * _signalInfo.timePerSample() * 1000.0f;

  real_starttime = Timing::time();

  float cumul_load = 0.0f;
  engine->_averageLoad=0.0f;

  int currentFrame = 0;

#ifndef SINGLE_THREADED_PLAYBACK
  std::list<Gear*> allGears = mainSchema->getDeepGears();
  for (std::list<Gear*>::iterator it=allGears.begin();it!=allGears.end();++it)
    (*it)->internalPrePlay();

  while (engine->_playing)
  {
#endif
    block_starttime = Timing::time();

    mainSchema->lock();

    engine->_orderedGears = mainSchema->getDeepOrderedReadyGears();

    //process audio
    for (std::list<Gear*>::iterator it=engine->_orderedGears.begin();it!=engine->_orderedGears.end();++it)
      (*it)->runAudio();

    //process video
    
    if (real_time >= (currentFrame*_videoInfo.timePerFrame()*1000.0f))
    {
      //MidiEngine::getInstance().purgeAndGetNew();

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

    mainSchema->unlock();
    engine->performScheduledGearUpdateSettings();
#ifndef SINGLE_THREADED_PLAYBACK
  }
  allGears = mainSchema->getDeepGears();
  for (std::list<Gear*>::iterator it=allGears.begin();it!=allGears.end();++it)
    (*it)->internalPostPlay();

#endif

  return NULL;
}


#ifdef SINGLE_THREADED_PLAYBACK

void Engine::debugStartPlay()
{
  _playing=true;
  std::list<Gear*> allGears = _mainMetaGear->getInternalSchema()->getDeepGears();
  for (std::list<Gear*>::iterator it=allGears.begin();it!=allGears.end();++it)
    (*it)->internalPrePlay();
}

void Engine::debugStopPlay()
{
  _playing=false;
  std::list<Gear*> allGears = _mainMetaGear->getInternalSchema()->getDeepGears();
  for (std::list<Gear*>::iterator it=allGears.begin();it!=allGears.end();++it)
    (*it)->internalPostPlay();
}
#endif

void Engine::scheduleGearUpdateSettings(Gear *gear)
{
  _scheduledsGearUpdateSettings.push_back(gear);

  if (!_playing)
    performScheduledGearUpdateSettings();
}

void Engine::performScheduledGearUpdateSettings()
{
  for (std::vector<Gear*>::iterator it=_scheduledsGearUpdateSettings.begin(); it!=_scheduledsGearUpdateSettings.end(); ++it)
  {    
    (*it)->updateSettings();
  }
  _scheduledsGearUpdateSettings.clear();
}

void Engine::onGearAdded(Schema *, Gear *gear)
{  
  if (_playing)
    gear->prePlay();  
}

void Engine::onGearRemoved(Schema *, Gear *gear)
{
  if (_playing)
    gear->postPlay();
}

