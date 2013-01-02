/* SignalType.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef SIGNALTYPE_INCLUDED
#define SIGNALTYPE_INCLUDED

#include "ArrayType.h"
#include "Engine.h"

typedef float Signal_T;
typedef Signal_T* Signal_T_ptr;

#define clipSignal(sig) ((sig < -1.0f) ? -1.0f : ((sig > 1.0f) ? 1.0f : sig))
#define signalToUChar(sig) ((unsigned char)((sig+1.0f)*128.0f))
#define signalToFrequency(sig) (sig*10000.0f)

class SignalType : public ArrayType<Signal_T>
{
public:
  SignalType(Signal_T fillValue = 0.0f)
  {
    resize(Engine::signalInfo().blockSize());
    std::fill(begin(), end(), fillValue);
  }

  virtual ~SignalType(){}

  TYPE_BASE_METHODS(SignalType, (249, 169, 7))
};

#endif 
