#ifndef CircBufSignal_included
#define CircBufSignal_included

#include "assert.h"
#include <list>
#include <string>

// this define code to cycle over samples from 'from' to 'to' 
// (relative to _current) of CircularBufferSignal object 'obj
// the variable pcur points on the actual sample

// e.g : 
// CIRCBUF_SIGNAL_T_FORBEGIN(b,-10,-1)  <- no semicolon
//   std::cerr<<*(cbptr++)<<",";
// CIRCBUF_SIGNAL_T_FOREND;


#define CIRCBUF_SIGNAL_T_FORBEGIN(obj,from,to)           \
    {                                                    \
      bool finished=false;                               \
      float *cbptr, *ptrlim, *cb_pa1,                \
                   *cb_pa2, *cb_pb1, *cb_pb2;               \
      obj->getBounds(from, to,                           \
                     cb_pa1, cb_pa2,                     \
                     cb_pb1, cb_pb2);                    \
      for(cbptr = cb_pa1, ptrlim = cb_pa2; !finished; )  \
      {


#define CIRCBUF_SIGNAL_T_FOREND                          \
        if(cbptr > ptrlim)                               \
          if(ptrlim == cb_pa2)                           \
            if(cb_pb1 > cb_pb2)                          \
              finished=true;                             \
            else                                         \
            {                                            \
              cbptr = cb_pb1;                            \
              ptrlim = cb_pb2;                           \
            }                                            \
          else finished=true;                            \
      }                                                  \
    }






const int _DYN_RESIZE_MAX=65536;

class CircularBufferSignal
{

public:

  enum Stats
    {
      STAT_NONE, STAT_SUM,STAT_SUMSQUARE
    };

  CircularBufferSignal(float def, int initsize=1024);

  void resize(int size);
  // returns 2 float * pairs : a1-a2 b1-b2 that covers the time span specified by t1,t2
  // where t1<=0 and t2<=0, specifying bounds of a sample window in the past ( e.g : t1= -512, t2= -256 ) 
  // (we need 2 pairs in case the time window crosses the buffer start.)
  void getBounds(int t1, int t2, float*& a1, float*& a2, float*& b1, float*& b2, Stats which_stat=STAT_NONE);

  void append(float * ptr, int size);
  // appends and enlarge if buffer is not big enough
  void appendEnlarge(float * ptr, int size);

  void setDynamicResizingMaximumSize(int sz);

  // satistics are 1/3 implemented.. do not use
  void setStat(Stats stat, bool computed);


private:

  // the default value used to fill in 
  float _default;

  // pointer on the beginning of the buffer
  float *_buf;

  // pointer on the beginning of the buffer of sums
  float *_bufSum;

  // pointer on the beginning of the buffer of sums^2
  float *_bufSumSquare;


  // total number of samples in the circ buffer
  int _bufSize;


  // max size of dynamic resizing 
  // (that is, resizing that occurs when getBounds 1st two params span more time than the buffer has. New space is filled with default)
  int _maxDynSize;

  // index of next sample to be written 
  // (so most recent sample is _current-1
  float *_current; 

  bool _computeSum;
  bool _computeSumSquare;


};

#endif
