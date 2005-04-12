/* Engine.h
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

#ifndef ENGINE_INCLUDED
#define ENGINE_INCLUDED

#include <list>
#include <vector>
#include <string>
#include <pthread.h>

#include "SignalInfo.h"
#include "VideoInfo.h"
#include "Math.h"
#include "error.h"
#include "Schema.h"
#include <sys/time.h>


class Gear;
class AbstractPlug;
class QDomDocument;
class QDomElement;
class MetaGear;

class Engine  
{

public:

  Engine(int hwnd);
  virtual ~Engine();

  static const SignalInfo& signalInfo() {return _signalInfo;};
  static const VideoInfo& videoInfo() {return _videoInfo;};

  MetaGear* mainMetaGear() {return _mainMetaGear;}

  void startPlaying();
  void stopPlaying();

  // This returns the time in seconds since the play button was
  // started. Warning : the time returned NOT the real time : it is
  // incremented each frame by the normal duration of a frame. So if your
  // CPU is overloaded, this is not accurate at all use currentRealTime
  static Time_T currentTime();
  // returns number of sec (at milliseconds resolution) sinc 1 Jan 1970 00:00:00 UTC
  static float currentRealTime();

  int hWnd(){return _hWnd;};
  static void *playThread(void *parent);

  void needSynch() {_graphSynched=false;};

  void scheduleGearUpdateSettings(Gear *gear);

#ifdef SINGLE_THREADED_PLAYBACK  
  void debugStartPlay();
  void debugStopPlay();
#endif

  static bool _playing;

protected:

  void performScheduledGearUpdateSettings();

  int _hWnd;

  float _averageLoad;

  static SignalInfo _signalInfo;
  static VideoInfo _videoInfo;

  static Time_T _currentTime;


private:

  MetaGear *_mainMetaGear;
    
  bool _graphSynched;
  
  //is sorted in processing order when _GraphSynched==true
  //only contain gears->ready()    
  std::list<Gear*> _orderedGears;

  pthread_t _playThreadHandle;

  std::vector<Gear*> _scheduledsGearUpdateSettings;

};

#endif
