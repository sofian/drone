#include "CircularBufferSignal.h"
#include <string.h>
#include "Engine.h"

// initsize : size at first initialisation
CircularBufferSignal::CircularBufferSignal(float def, int initsize)
  :_default(def),_buf(NULL),_bufSize(0),_computeSum(false),_computeSumSquare(false)
{
  resize(initsize);
  setDynamicResizingMaximumSize(_DYN_RESIZE_MAX);
}


// stats not implemented yet
void CircularBufferSignal::setStat(Stats stat, bool computed)
{
  if (stat==STAT_SUM)
  {
    if (!_computeSum)
      resize(_bufSize);
    else
      delete[]_bufSum;
    _computeSum = computed;
  } else if (stat==STAT_SUMSQUARE)
  {
    if (!_computeSumSquare)
      resize(_bufSize);
    else
      delete[]_bufSumSquare;
    _computeSumSquare = computed;
  } else
    // log something
    assert(0);
}

// set to -1 to disable dyn. resizing
void CircularBufferSignal::setDynamicResizingMaximumSize(int sz)
{
  _maxDynSize=sz;
}


void CircularBufferSignal::resize(int newsize)
{
  // LOG something maybe?
  // we don't enable down-sizing the CircularBuffer for now 
  if (newsize<_bufSize)
    return;

  // make a new buffer
  float *newbuf = new float[newsize];

  // if we already had a buffer, we copy past signal
  // into the new one to avoid 'temporary amnesia'
  if (_bufSize)
  {
    float *nptr = newbuf;
    int newSamples = newsize-_bufSize;

    //fill new extra-space with default value
    for (int i=0;i<newSamples;i++)
      *(nptr++)=_default;

    CIRCBUF_SIGNAL_T_FORBEGIN(this,-_bufSize,-1)
      *(nptr++)= *(cbptr++);
    CIRCBUF_SIGNAL_T_FOREND

      }

  _bufSize=newsize;
  delete[] _buf;

  _buf = newbuf;
  _current=_buf;

  /* TODO
  // resize sum buf
  if(_computeSum)
  {
  // make a new buffer
  float* newbuf = new float[newsize];
    
  // if we already had a buffer, we copy past signal
  // into the new one to avoid 'temporary amnesia'
  if(_bufSize)
  {
  float* nptr = newbuf;
  int newSamples = newsize-_bufSize;
      
  //fill new extra-space with default value
  for(int i=0;i<newSamples;i++)
  *(nptr++)=_default;
      
  CIRCBUF_SIGNAL_T_FORLOOP(this,-_bufSize,-1)
  *(nptr++)= *(cbptr++);
  CIRCBUF_SIGNAL_T_FORLOOP_END
  }

  _bufSize=newsize;
  delete[] _buf;

  _buf = newbuf;
  _current=_buf;
  */

}


// appends data to the circular buffer
// no resizing is done
void CircularBufferSignal::append(float *ptr, int size)
{
  int toEnd = _buf + _bufSize - _current;
  int firstRun = MIN(size,toEnd);

  if (size > _bufSize)
    //log_something !!
    _bufSize=_bufSize;


  memcpy(_current, ptr, firstRun * sizeof(float));
  _current += firstRun;
  size -= firstRun;
  ptr += firstRun;

  if (size)
    // some samples remaining...
  {
    memcpy(_buf, ptr, size * sizeof(float));
    _current = _buf + size;
  }

  if (_current >=_buf + _bufSize)
    _current -= _bufSize;

}

// appends data to the circular buffer
// if append size is too big, we resize the buffer.
void CircularBufferSignal::appendEnlarge(float *ptr, int size)
{
  if (size > _bufSize)
    resize(size);
  append(ptr,size);
}


// returns bounds needed to run over the specified data
// sample_from : first sample to be read. relative to _current 
// sample_to : last sample to be read. relative to _current
//   - e.g : sample_from = -1000 sample_to = -1 -> read the samples between and including the one written 1000 samples ago and the most recent one.
//           0 can't be use since it would mean reading the _current pointer for which we haven't wrote data yet.
//           (in fact, _current points on the oldest sample of the buffer)
// the pa1, pa2, pb1, pb2 returned are pointer that can be used like this : 
//   - go from pa1 to pa2 inclusively, then from pb1 to pb2 inclusively, while processing your samples along the way
//
// which stat specifies which buffer to use  
void CircularBufferSignal::getBounds(int sample_from, int sample_to, float*& pa1, float*& pa2, float*& pb1, float*& pb2, Stats which_stat)
{
  // sample_from/to can are [-x to -1] since 0 would mean getting t
  assert(sample_from < 0 && sample_to < 0);

  int samplesWanted = sample_to - sample_from + 1;

  // check if we need dynamic resizing
  if (samplesWanted > _bufSize)
    if (samplesWanted <= _maxDynSize)
    {
      resize(samplesWanted);
    } else
      // some kind of log/error
      assert(0);

  float *bt1 = _current + sample_from;
  float *bt2 = _current + sample_to;

  if ( bt1 >= _buf || (bt1 < _buf && bt2 < _buf) )
  {
    // the chunk is in one single piece
    if (bt2<_buf)
    {
      // the piece is before buffer start so we transpose it
      bt1+=_bufSize;
      bt2+=_bufSize;
    }

    pa1=bt1;
    pa2=bt2;  
    // make it so second chunk will never be processed
    pb1=bt2+1;
    pb2=bt2;

  } else
    // start is "before" buffer start but end is after (chunk is split in 2)
  {

    pa1 = bt1 + _bufSize;
    pa2 = _buf + _bufSize-1;

    pb1 = _buf;
    pb2 = _buf + (samplesWanted - (pa2 - pa1 + 2));
  }

  if (which_stat==STAT_NONE)
    return;
  else
  {
    int offset;
    if (which_stat==STAT_SUM)
      offset = (int)_bufSum-(int)_buf;
    else if (which_stat==STAT_SUMSQUARE)
      offset = (int)_bufSumSquare-(int)_buf;
    else
      // LOG SOMETHING
      assert(0);
    pa1+=offset;
    pa2+=offset;
    pb1+=offset;
    pb2+=offset;
  }    
}


