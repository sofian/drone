#ifndef SIGNALINFO_INCLUDED
#define SIGNALINFO_INCLUDED

#include "Typedefs.h"
#include "Timing.h"

class SignalInfo
{
  static const int DEFAULT_BLOCKSIZE;
  static const int DEFAULT_SAMPLERATE;

public:
  SignalInfo() : _blockSize(DEFAULT_BLOCKSIZE) {
    sampleRate(DEFAULT_SAMPLERATE);
  }

  void sampleRate(int samplerate){
    _sampleRate = samplerate;
    _timePerSample = 1.0f / (float)samplerate;
  }

  int blockSize() const {return _blockSize;}

  int sampleRate() const {return _sampleRate;}

  Time_T timePerSample() const {return _timePerSample;}

private:    
  int _blockSize;
  int _sampleRate;
  Time_T _timePerSample;    
};

#endif
