/* CircularBuffer.h
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

#ifndef CircBuf_included
#define CircBuf_included

#include "error.h"

// code to cycle over samples from 'from' to 'to' 
// (relative to this->_current) of CircularBufferSignal object 'obj
// the variable cbptr points on the actual sample

// e.g : (get 10 samples)
// CIRCBUF_FORBEGIN(b,-9,0)  <- no semicolon
//   std::cerr<<*(cbptr++)<<",";
// CIRCBUF_FOREND;


#define CIRCBUF_FORBEGIN(Type,obj,from,to)               \
    {                                                    \
      bool finished=false;                               \
      Type *cbptr, *ptrlim, *cb_pa1,                     \
                   *cb_pa2, *cb_pb1, *cb_pb2;            \
      obj->getBounds(from, to,                           \
                     cb_pa1, cb_pa2,                     \
                     cb_pb1, cb_pb2);                    \
      for(cbptr = cb_pa1, ptrlim = cb_pa2; !finished; )  \
      {


#define CIRCBUF_FOREND                                   \
        if(cbptr > ptrlim) {                             \
          if(ptrlim == cb_pa2)                           \
            if(cb_pb1 > cb_pb2)                          \
              finished=true;                             \
            else                                         \
            {                                            \
              cbptr = cb_pb1;                            \
              ptrlim = cb_pb2;                           \
            }                                            \
          else finished=true;                            \
        }                                                \
      }                                                  \
    }

//! This is a circular (or ring) buffer class that stores chunks of basic elements (char, float, ...)

template <class T>
class CircularBuffer
{

public:

//   enum Stats
//     {
//       STAT_NONE, STAT_SUM,STAT_SUMSQUARE
//     };

  CircularBuffer(T def)
    :_default(def),_buf(NULL),_blockSize(0),_numBlocks(0),_bufSize(0)//,_computeSum(false),_computeSumSquare(false)
    {
      resize(_blockSize,_numBlocks);
    }

  void resize(int blockSize, int numBlocks)
    {
      if(blockSize == _blockSize && numBlocks == _numBlocks)
        return;
      int newsize = blockSize * numBlocks;
      int oldBlockSize=_blockSize;
      _blockSize = blockSize;
      _numBlocks = numBlocks;

      // LOG something maybe?
      // we don't enable down-sizing the CircularBuffer for now
      if (newsize<_bufSize)
      {
        WARNING("CircularBuffer: Downsizing is not yet implemented");
        return;
      }
      // make a new buffer
      T *newbuf = new T[newsize];
      
      // if we already had a buffer, we copy past signal
      // into the new one to avoid 'temporary amnesia'
      if (_bufSize && oldBlockSize == blockSize)
      {
        T *nptr = newbuf;
        int newSamples = newsize-_bufSize;
        
        //fill new extra-space with default value
        for (int i=0;i<newSamples;i++)
          *(nptr++)=_default;
        
        CIRCBUF_FORBEGIN(T,this,-_bufSize+1,0)
          *(nptr++)= *(cbptr++);
        CIRCBUF_FOREND
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


  //! gets a pointer to a previously appended block
  //! sample is taken between  -numBlocks+1..0 inclusively
  T* getBlock(int sample)
    {
      ASSERT_WARNING(sample <= 0 && sample >= -_numBlocks+1);
      sample = CLAMP(sample,-_numBlocks+1,0);
      sample--;
      T* ptr = _current + sample * _blockSize;
      if(ptr<_buf)
        ptr+=_bufSize;

      //NOTICE("cur: %i sample %i ptr %i buf %i nblk %i bksz: %i bufsz: %i",_current,sample,ptr,_buf,_numBlocks,_blockSize,_bufSize);
      return ptr;
      
    }

  void fillVectorFromBlock(std::vector<T>* vec, int sample)
    {
      ASSERT_ERROR((int)vec->size() == _blockSize);
      memcpy(&(vec->front()), getBlock(sample), sizeof(T) * _blockSize);
    }

  
  // returns bounds needed to run over the specified data
  // sample_from : first sample to be read. relative to _current 
  // sample_to : last sample to be read. relative to _current
  //   - e.g : sample_from = -999 sample_to = 0 -> read the samples between and including the one written 1000 samples ago and the most recent one.
  // the pa1, pa2, pb1, pb2 returned are pointer that can be used like this : 
  //   - go from pa1 to pa2 inclusively, then from pb1 to pb2 inclusively, while processing your samples along the way
  //
  // which stat specifies which buffer to use  
  void getBounds(int sample_from, int sample_to, T*& pa1, T*& pa2, T*& pb1, T*& pb2)
    {
      // this simplifies computations
      sample_from--;
      sample_to--;
      // sample_from/to are [-x to -1] since 0 would mean getting t
      ASSERT_ERROR(sample_from < 0 && sample_to < 0);
      
      int samplesWanted = sample_to - sample_from + 1;
      
      // check if we need dynamic resizing
      ASSERT_WARNING(samplesWanted > _bufSize);
      
      samplesWanted=CLAMP(samplesWanted,0,_bufSize);

      T *bt1 = _current + sample_from;
      T *bt2 = _current + sample_to;
      
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
      

//       if (which_stat==STAT_NONE)
//         return;
//       else
//       {
//         int offset;
//         if (which_stat==STAT_SUM)
//           offset = (int)_bufSum-(int)_buf;
//         else if (which_stat==STAT_SUMSQUARE)
//           offset = (int)_bufSumSquare-(int)_buf;
//         else
//           // LOG SOMETHING
//           assert(0);
//         pa1+=offset;
//         pa2+=offset;
//         pb1+=offset;
//         pb2+=offset;
//       }    
    }
  


// fonction qui append une nombre quelconque de samples


//   // appends data to the circular buffer
//   // no resizing is done
//   void appendBuffer(const T *ptr, int size)
//     {
//       // number of samples to end of buffer
//       int toEnd = _buf + _bufSize - _current;
//       // contiguous samples in first pass
//       int firstRun = MIN(size,toEnd);
      
//       // are we appending more than the buffer
//       if (size > _bufSize)
//         //log_something !!
//         _bufSize=_bufSize;
      
      
//       memcpy(_current, ptr, firstRun * sizeof(float));
//       _current += firstRun;
//       size -= firstRun;
//       ptr += firstRun;
      
//       if (size)
//         // some samples remaining...
//       {
//         memcpy(_buf, ptr, size * sizeof(float));
//         _current = _buf + size;
//       }
      
//       if (_current >=_buf + _bufSize)
//         _current -= _bufSize;
      
//     }

  // appends data to the circular buffer
  // no resizing is done
  void append(const T *ptr)
    {
      memcpy(_current, ptr, _blockSize * sizeof(T));
      _current += _blockSize;
      
      if (_current >=_buf + _bufSize)
        _current -= _bufSize;
      
    }

//   // satistics are 1/3 implemented.. do not use
//   void setStat(Stats stat, bool computed);

  inline int numBlocks() const {return _numBlocks;}
  inline int blockSize() const {return _blockSize;}
  inline int size() const {return _bufSize;}


private:

  // the default value used to fill in 
  T _default;

  // pointer on the beginning of the buffer
  T *_buf;

//   // pointer on the beginning of the buffer of sums
//   T *_bufSum;

//   // pointer on the beginning of the buffer of sums^2
//   T *_bufSumSquare;

  //! number of elements in a block 
  int _blockSize;
  int _numBlocks;

  //! total number of elements in the circ buffer
  int _bufSize;


  // index of next element to be written 
  // (so most recent sample is _current-1
  T *_current; 

//   bool _computeSum;
//   bool _computeSumSquare;


};

#endif
