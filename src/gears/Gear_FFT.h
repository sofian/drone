#ifndef GEAR_FFT_INCLUDED
#define GEAR_FFT_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "FFTType.h"
template<class T>
class CircularBuffer;

#include <fftw3.h>

class Gear_FFT : public Gear
{
public:
  Gear_FFT();
  virtual ~Gear_FFT();

  bool ready();  
  void runVideo();

protected:
  void internalInit();

private:  
  static const int DEFAULT_FFT_SIZE;  

  PlugIn<SignalType> *_AUDIO_IN;  
  PlugOut<FFTType> *_FFT_OUT;  

  //fftw
  double *_fftIn;        
  fftw_plan _fftPlan;

  double *_hammingWindow;

  CircularBuffer<Signal_T> *_circBuf;



};

#endif
