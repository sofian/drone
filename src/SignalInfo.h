#ifndef SIGNALINFO_INCLUDED
#define SIGNALINFO_INCLUDED

#include "Typedefs.h"
#include "Timing.h"

class SignalInfo
{
  static const int DEFAULT_BLOCKSIZE;
  static const int DEFAULT_SAMPLERATE;

public:
  SignalInfo() : _BlockSize(DEFAULT_BLOCKSIZE) {
    SampleRate(DEFAULT_SAMPLERATE);
  }

  void SampleRate(int samplerate){
    _SampleRate = samplerate;
    _TimePerSample = 1.0f / (float)samplerate;
  }

  int blockSize() const {return _BlockSize;}

  int sampleRate() const {return _SampleRate;}

  Time_T timePerSample() const {return _TimePerSample;}

private:    
  int _BlockSize;
  int _SampleRate;
  Time_T _TimePerSample;    
};

#endif
