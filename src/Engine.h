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

class Gear;
class AbstractPlug;
class QDomDocument;
class QDomElement;

class Engine  
{

public:

  class ScheduledConnectDisconnect
  {
  public:
    enum eConnectDisconnect
      {
        CONNECT, DISCONNECT
      };

    ScheduledConnectDisconnect(AbstractPlug *a, AbstractPlug *b, eConnectDisconnect conn) :
      _a(a),
      _b(b),
      connectDisconnect(conn)
    {
    };
    
    AbstractPlug *_a;
    AbstractPlug *_b;
    eConnectDisconnect connectDisconnect;//connection or disconnection
  };


  Engine(int hwnd);
  virtual ~Engine();

  static const SignalInfo& signalInfo() {return _signalInfo;};
  static const VideoInfo& videoInfo() {return _videoInfo;};

  std::string getNewGearName(std::string prefix);

  Schema* mainSchema() {return &_mainSchema;}

  void loadMainSchema(std::string filename);
  void saveMainSchema(std::string filename);
  void clearMainSchema();

  void startPlaying();
  void stopPlaying();


  Time_T currentTime();

  int hWnd(){return _hWnd;};
  static void *playThread(void *parent);

  void needSynch() {_graphSynched=false;};

  void scheduleConnection(AbstractPlug *plugA, AbstractPlug *plugB);
  void scheduleDisconnection(AbstractPlug *plugA, AbstractPlug *plugB);
  void scheduleGearDeletion(Gear *gear);
  void scheduleGearUpdateSettings(Gear *gear);


protected:

  void performScheduledConnectDisconnect();
  void performScheduledGearDeletion();
  void performScheduledGearUpdateSettings();
  void performAllScheduledTasks();

  int _hWnd;

  float _averageLoad;

  static SignalInfo _signalInfo;
  static VideoInfo _videoInfo;

  Time_T _currentTime;


private:

  Schema _mainSchema;

  bool _playing;
  bool _graphSynched;
  std::list<Gear*> _gears;

  //is sorted in processing order when _GraphSynched==true
  //only contain gears->ready()    
  std::list<Gear*> _orderedGears;

  pthread_t _playThreadHandle;

  std::vector<ScheduledConnectDisconnect> _scheduledsConnectDisconnect;
  std::vector<Gear*> _scheduledsGearDeletion;
  std::vector<Gear*> _scheduledsGearUpdateSettings;

};

#endif
