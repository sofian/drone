#ifndef GEAR_MEDIANFILTER_INCLUDED
#define GEAR_MEDIANFILTER_INCLUDED


#include "Gear.h"
#include "SummedAreaTable.h"


class Gear_MedianFilter : public Gear
{
public:

  Gear_MedianFilter(Engine *engine, std::string name);
  virtual ~Gear_MedianFilter();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN;
  //PlugVideoIn *_AMOUNT_MAP_IN;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;

  //local var
  Video_T *_image;     
  Video_T *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;

  int _x1,_y1,_x2,_y2;

  int _medianSize;
  int _halfMedianFilterSize;

  unsigned char *_iterMedianSelect;
  unsigned char *_iterData;

  unsigned char _medianSelect[1024];

  static inline unsigned char quickSelect(unsigned char arr[], int n);
};

/* * This Quickselect routine is based on the algorithm described in
 * "Numerical recipies in C", Second Edition,
 * Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5 */

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


#endif
