/* FrequencyAnalyzer.h
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

#ifndef FREQUENCYANALYZER_INCLUDED
#define FREQUENCYANALYZER_INCLUDED

#include "Plug.h"
#include <fftw3.h>

class FrequencyAnalyser
{
public:

  FrequencyAnalyser(int bufferSize);
  ~FrequencyAnalyser();

  void prepare(int bufferSize);

  void frequencyToTime(const float *inputBuffer, float *outputBuffer);
  void timeToFrequency(const float *inputBuffer, float *outputBuffer);
  void filter(const float *inputBuffer, float *outputBuffer);

  int getMasterFrequency();
  float getBand(int lowerFreq, int higherFreq);
  //fftw_real *powerSpectrum() {return _powerSpectrum;};


private:    
    fftw_plan _fftwPlan;
    fftw_plan _fftwPlanInvert;
    float *_hammingWindow;
    int _bufferSize;
    int _windowSize;

    fftw_complex *_inputComplex;//the complex buffer where our input will be put
    fftw_complex *_outputComplex;//the result of fftw
    fftw_complex *_backComplex;//the complex buffer where our input will be put

};


#endif
