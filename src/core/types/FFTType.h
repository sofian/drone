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

#ifndef FFTTYPE_INCLUDED
#define FFTTYPE_INCLUDED

#include "AbstractType.h"
#include "Engine.h"
#include <fftw3.h>

class FFTType : public AbstractType
{
public:
  FFTType(int fftSize=Engine::signalInfo().blockSize()) :
		AbstractType("FFT"),
    _fftSize(fftSize),
    _spectrumSize(fftSize/2+1)
  {
    _fftOut = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);  
  }

  virtual ~FFTType()
  {
    fftw_free(_fftOut);
  }

  TYPE_BASE_METHODS(FFTType, (9, 169, 7))

  double power(int bin) const
  {
    if (bin < 0 || bin > _spectrumSize)
      return 0.0;
    
    return 2.0*sqrt(_fftOut[bin][0]*_fftOut[bin][0] + _fftOut[bin][1]*_fftOut[bin][1]);
    
  }

  fftw_complex *fftOut() const {return _fftOut;}

  int spectrumSize() const {return _spectrumSize;}
  int fftSize() const {return _fftSize;}
  
private:
  fftw_complex *_fftOut;
  int _fftSize;
  int _spectrumSize;
};

#endif 
