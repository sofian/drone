/* Gear_MedianFilter.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_MEDIANFILTER_INCLUDED
#define GEAR_MEDIANFILTER_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"

/**
 * A not-that-efficient median filter. The main element of the code is a quick
 * median selector that was taken from http://ndevilla.free.fr/median/median/src/quickselect.c,
 * the which is based on the algorithm described in "Numerical recipies in C", Second Edition,
 * Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5.
 * 
 * @author Jean-Sébastien Senécal, Nicolas Devillard
 * @version %I% %G%
 */
class Gear_MedianFilter : public Gear
{
public:
  //! Default constructor.
  Gear_MedianFilter(Schema *schema, std::string uniqueName);

  // Destructor.
  virtual ~Gear_MedianFilter();

  void runVideo();
	

private:
  //! Video input.
  PlugIn<VideoRGBAType> *_VIDEO_IN;

  //! Video output.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  //! Radius of the kernel (e.g. 1 to use a 3x3 kernel, 2 to use a 5x5, etc.)
  PlugIn<ValueType> *_AMOUNT_IN;

  // Internal use.
  const VideoRGBAType *_image;     
  VideoRGBAType *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;

  int _x1,_y1,_x2,_y2;

  int _medianSize;
  int _halfMedianFilterSize;

  unsigned char *_iterMedianSelect;
  unsigned char *_iterData;

  unsigned char _medianSelect[15];

  static inline unsigned char quickSelect(unsigned char arr[], int n);
};

// This code by Nicolas Devillard - 1998. Public domain.

#define ELEM_SWAP(a,b) { register unsigned char t=(a); (a)=(b); (b)=t; }

unsigned char Gear_MedianFilter::quickSelect(unsigned char arr[], int n)
{
  register int low, high ;
  register int median;
  register int middle, ll, hh;
  low = 0 ;
  high = n-1 ;
  median = (low + high) / 2;
  for (;;)
  {
    if (high <= low)
      // One element only 
      return arr[median] ;

    if (high == low + 1)
    {
      // Two elements only 
      if (arr[low] > arr[high])
        ELEM_SWAP(arr[low], arr[high]) ;
      return arr[median] ;
    }

    // Find median of low, middle and high items; swap into position low 
    middle = (low + high) / 2;
    if (arr[middle] > arr[high])
      ELEM_SWAP(arr[middle], arr[high]) ;
    if (arr[low] > arr[high])
      ELEM_SWAP(arr[low], arr[high]) ;
    if (arr[middle] > arr[low])
      ELEM_SWAP(arr[middle], arr[low]) ;

    // Swap low item (now in position middle) into position (low+1) 
    ELEM_SWAP(arr[middle], arr[low+1]) ;

    // Nibble from each end towards middle, swapping items when stuck 
    ll = low + 1;
    hh = high;
    for (;;)
    {
      do ll++; while (arr[low] > arr[ll]) ;
      do hh--; while (arr[hh] > arr[low]) ;
      if (hh < ll)
        break;
      ELEM_SWAP(arr[ll], arr[hh]) ;
    }

    // Swap middle item (in position low) back into correct position 
    ELEM_SWAP(arr[low], arr[hh]) ;

    // Re-set active partition 
    if (hh <= median)
      low = ll;
    if (hh >= median)
      high = hh - 1;
  }
}

#undef ELEM_SWAP 


#endif
