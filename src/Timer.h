/* Timer.h
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

// Copyright (C) 2003 Ronan Collobert (collober@idiap.ch)
//                
// This file is part of Torch 3.
//
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef TIMER_INC
#define TIMER_INC

/** Timer... to measure time.
    It accumulates the time measured in several #resume()# (or
    constructor definition) and #stop()# calls.

    Use #getTime()# to know this accumulated time.

    @author Ronan Collobert (collober@idiap.ch)
*/
typedef float real;
class Timer
{
#ifdef _MSC_VER
private:
  static time_t _baseTime;
#endif
public:
  bool _isRunning;
  real _totalTime;
  real _startTime;

  /// Create the timer and start it now!
  Timer();

  /** Reset the timer. The timer will count time starting
      from now, and the accumulated time is erased.
  */
  void reset();

  /// Stop the timer. Updates accumulated time.
  void stop();

  /// Resume the timer. It will count time starting from now.
  void resume();

  /** Get the total accumulated time. (Until now, if the
      timer is still running.
  */
  real getTime();

  /** System dependent function which returns time elapsed
      since an arbitrary point reference in the past.
  */
  static real getRunTime();

  ~Timer();
};

#endif
