/* SignalInfo.h
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

#ifndef SIGNALINFO_INCLUDED
#define SIGNALINFO_INCLUDED

#include "Typedefs.h"
#include "Timing.h"

class SignalInfo
{
  static const int DEFAULT_BLOCKSIZE;
  static const int DEFAULT_SAMPLERATE;

public:
  SignalInfo() : _blockSize(DEFAULT_BLOCKSIZE) {
    sampleRate(DEFAULT_SAMPLERATE);
  }

  void sampleRate(int samplerate){
    _sampleRate = samplerate;
    _timePerSample = 1.0f / (Time_T)samplerate;
    _timePerBlock = _blockSize * _timePerSample;
  }

  int blockSize() const {return _blockSize;}

  int sampleRate() const {return _sampleRate;}

  Time_T timePerSample() const {return _timePerSample;}

  Time_T timePerBlock() const {return _timePerBlock;}

private:    
  int _blockSize;
  int _sampleRate;
  Time_T _timePerSample;
  Time_T _timePerBlock;
};

#endif
