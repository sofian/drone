/* Gear_SelectFrame.h
 * Copyright (C) 2004--2005 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef GEAR_SELECTFRAME_INCLUDED
#define GEAR_SELECTFRAME_INCLUDED

#include "Gear.h"
#include "ValueType.h"
#include "EnumType.h"

class Gear_SelectFrame : public Gear
{
public:
  enum ePlaybackMode
  {
    LOOPFORWARD,
    LOOPBACKWARD,
    FORWARD,
    BACKWARD,
    PING_PONG,
    RANDOM,
    N_PLAYBACK_MODE
  };
  
  Gear_SelectFrame(Schema *schema, std::string uniqueName);
  virtual ~Gear_SelectFrame();

  void runVideo();

private:
  // Inputs.
  PlugIn<ValueType>  *_NEXT_IN;
  PlugIn<ValueType>  *_RESET_IN;
  //  PlugIn<ValueType>  *_RESET_IN;
  PlugIn<ValueType>  *_N_FRAMES_IN;
  PlugIn<ValueType>  *_SEEK_IN;
  PlugIn<EnumType>   *_MODE_IN;

  // Outputs.
  PlugOut<ValueType> *_FRAME_OUT;

  //locals
  int _nFrames;
  int _currentFrame;
  int _currentSeekFrame;
  bool _next;

protected:
  // functions
  void internalInit();
};

#endif
