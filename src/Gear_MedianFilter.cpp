#include "Gear_MedianFilter.h"

#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

/* * This Quickselect routine is based on the algorithm described in
 * "Numerical recipies in C", Second Edition,
 * Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5 */

#define ELEM_SWAP(a,b) { register unsigned char t=(a); (a)=(b); (b)=t; }

unsigned char quick_select(unsigned char arr[], int n)
{
  int low, high ;
  int median;
  int middle, ll, hh;
  low = 0 ;
  high = n-1 ;
  median = (low + high) / 2;
  for (;;)
  {
    if (high <= low)
      /* One element only */
      return arr[median] ;

    if (high == low + 1)
    {
      /* Two elements only */
      if (arr[low] > arr[high])
        ELEM_SWAP(arr[low], arr[high]) ;
      return arr[median] ;
    }

    /* Find median of low, middle and high items; swap into position low */
    middle = (low + high) / 2;
    if (arr[middle] > arr[high])
      ELEM_SWAP(arr[middle], arr[high]) ;
    if (arr[low] > arr[high])
      ELEM_SWAP(arr[low], arr[high]) ;
    if (arr[middle] > arr[low])
      ELEM_SWAP(arr[middle], arr[low]) ;

    /* Swap low item (now in position middle) into position (low+1) */
    ELEM_SWAP(arr[middle], arr[low+1]) ;

    /* Nibble from each end towards middle, swapping items when stuck */
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

    /* Swap middle item (in position low) back into correct position */
    ELEM_SWAP(arr[low], arr[hh]) ;

    /* Re-set active partition */
    if (hh <= median)
      low = ll;
    if (hh >= median)
      high = hh - 1;
  }
}

#undef ELEM_SWAP 


Register_Gear(MAKERGear_MedianFilter, Gear_MedianFilter, "MedianFilter")

Gear_MedianFilter::Gear_MedianFilter(Engine *engine, std::string name) : Gear(engine, "MedianFilter", name)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("Amount", 1);
}

Gear_MedianFilter::~Gear_MedianFilter()
{
}

bool Gear_MedianFilter::ready()
{
  return (_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_MedianFilter::runVideo()
{
  _image = _VIDEO_IN->canvas();
  
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());
  _sizeY = _image->sizeY();
  _sizeX = _image->sizeX();

  _data = (unsigned char*)_image->_data;    
  _outData = (unsigned char*)_outImage->_data;

  _medianSize = (int) _AMOUNT_IN->buffer()[0];
  
  ////////////////////////////
  
  for(int y=0;y<_sizeY;y++)
  {   
    for(int x=0;x<_sizeX;x++)
    {
      _x1 = x - _medianSize;
      _x2 = x + _medianSize;
      _y1 = y - _medianSize;
      _y2 = y + _medianSize;
      
      if(_x1 < 0)_x1 = 0;
      if(_x2 < 0)_x2 = 0;
      if(_y1 < 0)_y1 = 0;
      if(_y2 < 0)_y2 = 0;
      
      if(_x1 >= _sizeX)_x1 = _sizeX-1;
      if(_x2 >= _sizeX)_x2 = _sizeX-1;
      if(_y1 >= _sizeY)_y1 = _sizeY-1;
      if(_y2 >= _sizeY)_y2 = _sizeY-1;

      for (int z=0; z<4; ++z)
      {
        unsigned char *iterMedianSelect = _medianSelect;
        for (int ySub=_y1; ySub<=_y2; ++ySub)
          for (int xSub=_x1; xSub<=_x2; ++xSub)
            *iterMedianSelect++ = _data[ (ySub*_sizeX*4) + (xSub*4) + z ]; // *** the * 4 can be precomputer
        *_outData++ = quick_select(_medianSelect, (_y2-_y1+1)*(_x2-_x1+1));
      }
      
    }
    
  }
  
       
}
